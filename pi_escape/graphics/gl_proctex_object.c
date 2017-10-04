#include "gl_proctex_object.h"

#include <assert.h>
#include <string.h>
#include "globject.h"
#include "../../util/util.h"

void gl_proctex_object_init(GLProcTexObject *obj, Graphics *graphics, int program_index) {
    globject_init(&obj->gl_object, graphics, program_index);
    
    obj->graphics = graphics;
    obj->program_index = program_index;
    obj->program_id = get_program_id(graphics, program_index);
    
    obj->blend = 0;
    obj->no_cull = 0;
    
    //set some defaults. You want to override these in most cases.
    obj->minimumAmbiantLightMulti = 0.1f;
    obj->defaultAmbiantLightMulti = 0.2f;
    obj->diffuseLightMulti = 0.2f;
    obj->specularLightMulti = 0.3f;
    obj->proceduralTextureID = 0.0f;
    gl_set_color(obj->colorA, 1.0f, 0.0f, 0.0f, 1.0f);
    gl_set_color(obj->colorB, 0.0f, 1.0f, 0.0f, 1.0f);
    obj->proceduralTextureParameterA = 0.1f;
    obj->proceduralTextureParameterB = 0.1f;
    obj->proceduralTextureParameterC = 0.1f;
    obj->proceduralTextureParameterD = 0.1f;
    
    memset(&obj->vertex_buffer_data[0], -1000, sizeof(obj->vertex_buffer_data));
    memset(&obj->normal_buffer_data[0], -1000, sizeof(obj->normal_buffer_data));
    memset(&obj->texCoord_buffer_data[0], -1000, sizeof(obj->texCoord_buffer_data));
    
    assert(HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexBasePosition_modelspace));
//    assert(HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexNormal_modelspace));
//    assert(HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexTexCoord));
    
    glGenBuffers(1, &obj->vertexbuffer);
    glGenBuffers(1, &obj->normalbuffer);
    glGenBuffers(1, &obj->texcoordbuffer);
    handle_gl_error2("GLProcTexObject", "glGenBuffers()");
}

void gl_proctex_object_free(GLProcTexObject *obj) {
    glDeleteBuffers(1, &obj->vertexbuffer);
    handle_gl_error2("GLProcTexObject", "glDeleteBuffers() for VertexBasePosition_modelspace");
    
    glDeleteBuffers(1, &obj->normalbuffer);
    handle_gl_error2("GLProcTexObject", "glDeleteBuffers() for VertexNormal_modelspace");
    
    glDeleteBuffers(1, &obj->texcoordbuffer);
    handle_gl_error2("GLProcTexObject", "glDeleteBuffers() for VertexTexCoord");
    
    globject_free(&obj->gl_object);
}

void gl_proctex_init_vertexbuffers(GLProcTexObject *obj, int vertexCount, char *objName) {
    obj->vertexCount = vertexCount;
    assert(vertexCount <= GLLEVELPART_MAX_VERTICES);
    assert(vertexCount > 0);
    
    for (int i = 0; i < vertexCount * 3; i++) {
        GLfloat vertex = obj->vertex_buffer_data[i];
        //we expect all models to be max distance 1 from source
        if (vertex < -1.0 || vertex > 1.0) {
            fatal("Check failed: %s index=%d vertex=%f", objName, i, vertex);
        }
        
        //normals should be normalized
        GLfloat normal = obj->normal_buffer_data[i];
        if (normal < -1.0 || normal > 1.0) {
            fatal("Check failed: %s index=%d normal=%f", objName, i, normal);
        }
    }
    for (int i = 0; i < vertexCount * 2; i++) {
        GLfloat texCoord = obj->texCoord_buffer_data[i];
        //texture coordinates are always between 0.0 and 1.0
        if (texCoord < 0.0 || texCoord > 1.0) {
            fatal("Check failed: %s index=%d texCoord=%f", objName, i, texCoord);
        }
    }
    //indices should already be set, so we check if they all point to an existing vertex
    for (int i = 0; i < obj->gl_object.indicesCount; i++) {
        assert(obj->gl_object.indicesCount > 0);
        assert(obj->gl_object.indicesCount < GLOBJECT_MAX_INDICES);
        if (obj->gl_object.indices[i] < 0 || obj->gl_object.indices[i] >= vertexCount) {
            fatal("Invalid indices[%d] = %d while vertexCount=%d\n", i, obj->gl_object.indices[i], vertexCount);
        }
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * vertexCount, obj->vertex_buffer_data, GL_STATIC_DRAW);
    handle_gl_error2(objName, "glGenBuffers() glBindBuffer() glBufferData() for VertexBasePosition_modelspace");
    
    if (HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexNormal_modelspace)) {
        glBindBuffer(GL_ARRAY_BUFFER, obj->normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * vertexCount, obj->normal_buffer_data, GL_STATIC_DRAW);
        handle_gl_error2(objName,
                         "glGenBuffers() glBindBuffer() glBufferData() for VertexNormal_modelspace");
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, obj->texcoordbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * vertexCount, obj->texCoord_buffer_data, GL_STATIC_DRAW);
    handle_gl_error2(objName, "glGenBuffers() glBindBuffer() glBufferData() for VertexTexCoord");
}


void gl_proctex_draw_call(GLProcTexObject *obj) {
    if (obj->blend) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    if (obj->no_cull) {
        glCullFace(GL_FRONT);
    }

//    printf("Draw vertexbuffer=%d normalbuffer=%d texbuffer=%d elementbuffer=%d\n", vertexbuffer, normalbuffer, texbuffer, elementbuffer);
//    printf("Draw vertexCount=%d indicesCount=%d\n", vertexCount, indicesCount);
    glDrawElements(
            GL_TRIANGLES,      // mode
            obj->gl_object.indicesCount,   // count
            GL_UNSIGNED_SHORT,   // type
            (void*)0           // element array buffer offset
    );
    
    if (obj->no_cull) {
        glCullFace(GL_BACK);
        glDrawElements(
                GL_TRIANGLES,      // mode
                obj->gl_object.indicesCount,   // count
                GL_UNSIGNED_SHORT,   // type
                (void*)0           // element array buffer offset
        );
    }
    
    if (obj->blend) {
        glDisable(GL_BLEND);
    }
}

void gl_proctex_draw(GLProcTexObject* obj, t_vec3 translation) {
    //check if init done correctly
    assert(obj->gl_object.elementbuffer >= 0);
    assert(obj->gl_object.indicesCount > 0);
    assert(obj->gl_object.indicesCount < GLOBJECT_MAX_INDICES);
    assert(obj->vertexbuffer >= 0);
    assert(obj->vertexCount > 0);
    assert(obj->vertexCount <= GLOBJECT_MAX_VERTICES);
    
    glUseProgram(obj->program_id);
    
    GLint vertexPosID = GET_ATTRIBUTE_ID(obj->graphics, obj->program_index, VertexBasePosition_modelspace);
    glEnableVertexAttribArray(vertexPosID);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertexbuffer);
    glVertexAttribPointer(
            vertexPosID,                       // attribute
            3,                                    // size
            GL_FLOAT,                             // type
            GL_FALSE,                             // normalized?
            0,                                    // stride
            (void *) 0                              // array buffer offset
    );
    handle_gl_error("GLProcTexObject glEnableVertexAttribArray(vertexPosID)");
    
    if (HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexNormal_modelspace)) {
        GLint normalID = GET_ATTRIBUTE_ID(obj->graphics, obj->program_index, VertexNormal_modelspace);
        glEnableVertexAttribArray(normalID);
        glBindBuffer(GL_ARRAY_BUFFER, obj->normalbuffer);
        glVertexAttribPointer(
                normalID,                      // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void *) 0                        // array buffer offset
        );
        handle_gl_error("GLProcTexObject glEnableVertexAttribArray(normalID)");
    }
    
    if (HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexTexCoord)) {
        GLint texCoordID = GET_ATTRIBUTE_ID(obj->graphics, obj->program_index, VertexTexCoord);
        glEnableVertexAttribArray(texCoordID);
        glBindBuffer(GL_ARRAY_BUFFER, obj->texcoordbuffer);
        glVertexAttribPointer(
                texCoordID,                    // attribute
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void *) 0                        // array buffer offset
        );
        handle_gl_error("GLProcTexObject glVertexAttribPointer(texbuffer)");
    }
    
    
    //set uniforms specific for this object
    if (HAS_UNIFORM(obj->graphics, obj->program_index, PerDrawTransformation_worldspace)) {
        t_mat4 preTransformationMatrix;
        t_mat4 iden4;
        glmc_identity(iden4);
        glmc_translate(iden4, translation, preTransformationMatrix);
        
        glUniformMatrix4fv(GET_UNIFORM_ID(obj->graphics, obj->program_index, PerDrawTransformation_worldspace), 1, GL_FALSE, preTransformationMatrix);
        handle_gl_error("GLProcTexObject::draw() glUniformMatrix4fv() PerDrawTransformation_worldspace");
    }
    
    if (HAS_UNIFORM(obj->graphics, obj->program_index, TextureLightSettings)) {
        assert(obj->defaultAmbiantLightMulti >= 0.0f && obj->defaultAmbiantLightMulti <= 10.0f);
        assert(obj->minimumAmbiantLightMulti >= 0.0f && obj->minimumAmbiantLightMulti <= 10.0f);
        assert(obj->diffuseLightMulti >= 0.0f && obj->diffuseLightMulti <= 10.0f);
        assert(obj->specularLightMulti >= 0.0f && obj->specularLightMulti <= 10.0f);
        glUniform3f(GET_UNIFORM_ID(obj->graphics, obj->program_index, TextureLightSettings),
                    obj->defaultAmbiantLightMulti < obj->minimumAmbiantLightMulti ? obj->minimumAmbiantLightMulti : obj->defaultAmbiantLightMulti,
                    obj->diffuseLightMulti, obj->specularLightMulti);
        handle_gl_error("GLProcTexObject::draw() glUniform3f() TextureLightSettings");
    }
    
    if (HAS_UNIFORM(obj->graphics, obj->program_index, ProceduralTextureID)) {
        assert(obj->proceduralTextureID >= 0.0f && obj->proceduralTextureID <= 5.0f);
        glUniform1f(GET_UNIFORM_ID(obj->graphics, obj->program_index, ProceduralTextureID), obj->proceduralTextureID);
        handle_gl_error("GLProcTexObject::draw() glUniform1f() ProceduralTextureID");
    }
    
    if (HAS_UNIFORM(obj->graphics, obj->program_index, ColorA)) {
        assert(is_sensible_color(obj->colorA));
        glUniform4f(GET_UNIFORM_ID(obj->graphics, obj->program_index, ColorA), obj->colorA[0], obj->colorA[1], obj->colorA[2], obj->colorA[3]);
        handle_gl_error("GLProcTexObject::draw() glUniform4f() ColorA");
    }
    if (HAS_UNIFORM(obj->graphics, obj->program_index, ColorB)) {
        assert(is_sensible_color(obj->colorB));
        glUniform4f(GET_UNIFORM_ID(obj->graphics, obj->program_index, ColorB), obj->colorB[0], obj->colorB[1], obj->colorB[2], obj->colorB[3]);
        handle_gl_error("GLProcTexObject::draw() glUniform4f() ColorB");
    }
    
    if (HAS_UNIFORM(obj->graphics, obj->program_index, ProceduralTextureParameter)) {
        assert(obj->proceduralTextureParameterA >= 0.0f && obj->proceduralTextureParameterA <= 10.0f);
        assert(obj->proceduralTextureParameterB >= 0.0f && obj->proceduralTextureParameterB <= 10.0f);
        assert(obj->proceduralTextureParameterC >= 0.0f && obj->proceduralTextureParameterC <= 10.0f);
        assert(obj->proceduralTextureParameterD >= 0.0f && obj->proceduralTextureParameterD <= 10.0f);
        glUniform4f(GET_UNIFORM_ID(obj->graphics, obj->program_index, ProceduralTextureParameter),
                    obj->proceduralTextureParameterA, obj->proceduralTextureParameterB,
                    obj->proceduralTextureParameterC, obj->proceduralTextureParameterD);
        handle_gl_error("GLProcTexObject::draw() glUniform4f() ProceduralTextureParameter");
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->gl_object.elementbuffer);
    gl_proctex_draw_call(obj);
    handle_gl_error("GLProcTexObject glDrawElements()");
    
    glDisableVertexAttribArray(vertexPosID);
    if (HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexNormal_modelspace)) {
        GLint normalID = GET_ATTRIBUTE_ID(obj->graphics, obj->program_index, VertexNormal_modelspace);
        glDisableVertexAttribArray(normalID);
    }
    if (HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexTexCoord)) {
        GLint texCoordID = GET_ATTRIBUTE_ID(obj->graphics, obj->program_index, VertexTexCoord);
        glDisableVertexAttribArray(texCoordID);
    }
}
