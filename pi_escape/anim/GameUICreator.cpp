#include "GameUICreator.h"

#include "MenuBuilder.h"
#include "MovieBuilder.h"

GameUICreator::GameUICreator() {}


GameUICreator::~GameUICreator() {

}

//TODO: uncomment the code below, and make it work

//EntryBuilder& addMainMenuAnimation(EntryBuilder& entryBuilder) {
//    return entryBuilder.addAnimation(new SineAnimation(new MoveAnimation(150, 0))
//                    , ACTIVATE, MENUSTATECHANGE_START, false, 1000l)
//            .addAnimation(new ReverseAnimation(new FadeInAnimation())
//                    , OTHER_ACTIVATED, MENUSTATECHANGE_START, false, 1000l)
//            .addAnimation(new SineAnimation(new MoveAnimation(-400, 0))
//                    , OTHER_ACTIVATED, MENUSTATECHANGE_START, false, 1000l)
//            .addAnimation(
//                    new ReverseAnimation(
//                    new GlyphIteratingAnimation(
//                            new InOutAnimation(new SineAnimation(new MoveAnimation(0, 5))),
//                            1.5f))
//                    , DEFAULT, MENUSTATECHANGE_START, false, 500l)
//            .addAnimation(
//                    new GlyphIteratingAnimation(
//                            new InOutAnimation(new SineAnimation(new MoveAnimation(0, 10))),
//                            1.5f)
//                    , HOVER, MENUSTATECHANGE_START, true, 2000l)
//            .addAnimation(new RainbowColorAnimation(), HOVER, MENUSTATECHANGE_START, true, 1000l);
//}
//
//std::shared_ptr<MenuDefinition> GameUICreator::createGameMenu() {
//    MenuBuilder builder;
//
//    addMainMenuAnimation(builder.addEntry())
//            .setEnabledOnPc(true).setEnabledOnPi(true)
//            .setLongText("Start Tutorial")
//            .setShortText("Tut")
//            .setMnemonic('T')
//            .setFontName("arcade")
//            .buildEntryWithAction("start tutorial");
//
//    addMainMenuAnimation(builder.addEntry())
//            .setEnabledOnPc(true).setEnabledOnPi(true)
//            .setLongText("Start Game")
//            .setShortText("Go")
//            .setMnemonic('G')
//            .setFontName("arcade")
//            .buildEntryWithAction("start game");
//
//    addMainMenuAnimation(builder.addEntry())
//            .setEnabledOnPc(true).setEnabledOnPi(true)
//            .setLongText("Exit")
//            .setShortText("Exit")
//            .setMnemonic('E')
//            .setFontName("arcade")
//            .buildEntryWithAction("quit");
//
//    return std::shared_ptr<MenuDefinition>(builder.build());
//}
//
//std::shared_ptr<MovieDefinition> GameUICreator::createIntro() {
//    MovieBuilder builder;
//    builder.setDuration(10000l);
//
//    t_vec4 col1 = {1.0f, 1.0f, 0.0f, 1.0f};
//    t_vec4 col2 = {0.0f, 1.0f, 1.0f, 1.0f};
//
//    builder.addText("Sysprog 2017 presents:")
//            .setFont("arcade")
//            .setColor(col1)
//            .setPos_percent(50.0f, 75.0f)
//            .setStartTime(0000l).setEndTime(7000l)
//            .addAnimation(new ColorAnimation(1.0f, 0.0f, 0.0f),
//                          1000l, 1000l)
//            .addAnimation(new GlyphIteratingAnimation(
//                    new ColorAnimation(1.0f, 0.0f, 0.0f), 1.7f),
//                          1000l, 1000l)
//            .addAnimation(new GlyphIteratingAnimation(
//                    new MoveAnimation(0, 20), 3.0f),
//                          1000l, 2000l)
//            .addAnimation(new FadeInAnimation(), 0l, 2000l)
//            .endText();
//
//    builder.addText("PiEscape 2")
//            .setFont("zorque")
//            .setColor(col2)
//            .setPos_percent(50.0f, 25.0f)
//            .setStartTime(1000l).setEndTime(10000l)
//
//            .addAnimation(new ReverseAnimation(new FadeInAnimation()), 9000l, 1000l)
//
//            .addAnimation(new RepeatAnimation(
//                    new GlyphIteratingAnimation(
//                            new InOutAnimation(new SineAnimation(new MoveAnimation(0, 40))),
//                            1.5f),
//                    1, true, true, false
//            ), 3000l, 4000l)
//            .addAnimation(new RepeatAnimation(
//                    new GlyphIteratingAnimation(
//                            new RainbowColorAnimation(), 3.0f), 9),
//                          0000l, 9000l)
//            .addAnimation(new GlyphIteratingAnimation(
//                    new SineAnimation(new MoveAnimation(0, 400)), 1.0f),
//                          0000l, 3000l)
//            .addAnimation(
//                    new GlyphIteratingAnimation(new FadeInAnimation(), 1.0f)
//                    , 0000l, 3000l)
//            .endText();
//
//    return builder.build();
//}
//
//std::shared_ptr<MovieDefinition> GameUICreator::createCredits() {
//    MovieBuilder builder;
//
//    return builder.build();
//}
//
//std::shared_ptr<MovieDefinition> GameUICreator::createOutro() {
//    MovieBuilder builder;
//
//    builder.setDuration(10000l);
//
//    t_vec4 col1 = {1.0f, 1.0f, 0.0f, 1.0f};
//    t_vec4 col2 = {0.0f, 1.0f, 1.0f, 1.0f};
//
//    builder.addText("You have reached the end!")
//            .setFont("arcade")
//            .setColor(col1)
//            .setPos_percent(50.0f, 75.0f)
//            .setStartTime(0000l).setEndTime(7000l)
////            .addAnimation(new ColorAnimation(1.0f, 0.0f, 0.0f),
////                          AnimationType::IN_OUT, 1000l, 1000l)
//            .addAnimation(new GlyphIteratingAnimation(
//                    new ColorAnimation(1.0f, 0.0f, 0.0f), 1.7f),
//                          1000l, 1000l)
//            .addAnimation(new GlyphIteratingAnimation(
//                    new SineAnimation(new MoveAnimation(0, 20)), 3.0f),
//                          1000l, 2000l)
//            .addAnimation(new FadeInAnimation(), 0l, 2000l)
//            .endText();
//
//    builder.addText("Thank you for playing PiEscape 2.")
//            .setFont("zorque")
//            .setColor(col2)
//            .setPos_percent(50.0f, 25.0f)
//            .setStartTime(1000l).setEndTime(10000l)
//            .addAnimation(
//                    new RepeatAnimation(
//                            new GlyphIteratingAnimation(
//                                    new RainbowColorAnimation(), 10.5f),
//                            9, true, false, true)
//                    , 0l, 9000l)
//            .addAnimation(new GlyphIteratingAnimation(
//                    new SineAnimation(new MoveAnimation(0, 400)), 1.0f),
//                          0l, 3000l)
//            .addAnimation(new GlyphIteratingAnimation(
//                    new FadeInAnimation(), 1.0f),
//                          0l, 3000l)
//            .addAnimation(new ReverseAnimation(new FadeInAnimation()), 9000l, 1000l)
//            .endText();
//
//    return builder.build();
//}
