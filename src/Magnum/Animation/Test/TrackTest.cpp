/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <sstream>
#include <Corrade/TestSuite/Tester.h>

// #include "Magnum/Animation/Animator.h" /** @todo remove */
#include "Magnum/Animation/Interpolator.h"
#include "Magnum/Animation/Track.h"
#include "Magnum/Math/Color.h"
// #include "Magnum/Math/Quaternion.h"
// #include "Magnum/Trade/Animation.h" /** @todo also */

namespace Magnum { namespace Animation { namespace Test {

struct TrackTest: TestSuite::Tester {
    explicit TrackTest();

    void constructArray();
    void constructArrayDefaults();
    void constructInitializerList();
    void constructInitializerListDefaults();
    void constructView();
    void constructViewDefaults();

    void convertView();

    void at();
    void atSingleKeyframe();
    void atNoKeyframe();

    void atHint();

    void debugExtrapolation();
};

namespace {

enum: std::size_t { AtDataCount = 8 };

const struct {
    const char* name;
    Extrapolation extrapolationBefore;
    Extrapolation extrapolationAfter;
    Float time;
    Float expectedValue;
    std::size_t expectedHint;
} AtData[AtDataCount] {
    {"before default-constructed", Extrapolation::DefaultConstructed, Extrapolation::Extrapolated, -1.0f, 0.0f, 0},
    {"before constant", Extrapolation::Constant, Extrapolation::Extrapolated, -1.0f, 3.0f, 0},
    {"before extrapolated", Extrapolation::Extrapolated, Extrapolation::DefaultConstructed, -1.0f, 4.0f, 0},
    {"during first", {}, {}, 1.5f, 1.5f, 0},
    {"during second", {}, {}, 4.75f, 1.0f, 2},
    {"after default-constructed", Extrapolation::Extrapolated, Extrapolation::DefaultConstructed, 6.0f, 0.0f, 3},
    {"after constant", Extrapolation::Extrapolated, Extrapolation::Constant, 6.0f, 0.5f, 3},
    {"after extrapolated", Extrapolation::DefaultConstructed, Extrapolation::Extrapolated, 6.0f, -1.5f, 3}
};

enum: std::size_t { AtSingleKeyframeDataCount = 3 };

const struct {
    const char* name;
    Float time;
    Float expectedValue;
} AtSingleKeyframeData[AtSingleKeyframeDataCount] {
    {"before", -1.0f, 0.0f},
    {"at", 0.0f, 5.0f},
    {"after", 1.0f, 0.0f}};

enum: std::size_t { AtHintDataCount = 3 };

const struct {
    const char* name;
    std::size_t hint;
} AtHintData[AtHintDataCount] {
    {"before", 1},
    {"at", 2},
    {"after", 3}};

}

TrackTest::TrackTest() {
    addTests({&TrackTest::constructArray,
              &TrackTest::constructInitializerList,
              &TrackTest::constructView,

              &TrackTest::convertView});

    addInstancedTests({&TrackTest::at}, AtDataCount);

    addInstancedTests({&TrackTest::atSingleKeyframe}, AtSingleKeyframeDataCount);

    addTests({&TrackTest::atNoKeyframe});

    addInstancedTests({&TrackTest::atHint}, AtHintDataCount);

    addTests({&TrackTest::debugExtrapolation});
}

namespace {

enum class Target {
    Translation = 3,
    Color = 4,
    Weight = 5
};

template<class T> using Track = Animation::Track<Target, Float, T>;
template<class T> using TrackView = Animation::TrackView<Target, Float, T>;

}

using namespace Math::Literals;

void TrackTest::constructArray() {
    const Track<Vector3> a{Target::Translation, 5,
        Containers::Array<std::pair<Float, Vector3>>{Containers::InPlaceInit, {
            {0.0f, {3.0f, 1.0f, 0.1f}},
            {5.0f, {0.3f, 0.6f, 1.0f}}}},
        constantInterpolator<Target, Float>,
        Extrapolation::Extrapolated, Extrapolation::DefaultConstructed};

    CORRADE_VERIFY(a.target() == Target::Translation);
    CORRADE_COMPARE(a.index(), 5);
    CORRADE_COMPARE(a.interpolator(), (constantInterpolator<Target, Float>));
    CORRADE_COMPARE(a.before(), Extrapolation::Extrapolated);
    CORRADE_COMPARE(a.after(), Extrapolation::DefaultConstructed);
    CORRADE_COMPARE(a.data().size(), 2);
    CORRADE_COMPARE(a.data()[1].first, 5.0f);
}

void TrackTest::constructArrayDefaults() {
    {
        const Track<Vector3> a{Target::Translation, 5,
            Containers::Array<std::pair<Float, Vector3>>{Containers::InPlaceInit, {{0.0f, {3.0f, 1.0f, 0.1f}}}},
            linearInterpolator<Target, Float>, Extrapolation::DefaultConstructed};

        CORRADE_VERIFY(a.target() == Target::Translation);
        CORRADE_COMPARE(a.index(), 5);
        CORRADE_COMPARE(a.interpolator(), (linearInterpolator<Target, Float>));
        CORRADE_COMPARE(a.before(), Extrapolation::DefaultConstructed);
        CORRADE_COMPARE(a.after(), Extrapolation::DefaultConstructed);
        CORRADE_COMPARE(a.data().size(), 1);
    } {
        const Track<Color3> a{Target::Color,
            Containers::Array<std::pair<Float, Color3>>{Containers::InPlaceInit, {{0.0f, 0xfeadfb_rgbf}}},
            constantInterpolator<Target, Float>, Extrapolation::Extrapolated, Extrapolation::DefaultConstructed};

        CORRADE_VERIFY(a.target() == Target::Color);
        CORRADE_COMPARE(a.index(), 0);
        CORRADE_COMPARE(a.interpolator(), (constantInterpolator<Target, Float>));
        CORRADE_COMPARE(a.before(), Extrapolation::Extrapolated);
        CORRADE_COMPARE(a.after(), Extrapolation::DefaultConstructed);
        CORRADE_COMPARE(a.data().size(), 1);
    } {
        const Track<Float> a{Target::Weight,
            Containers::Array<std::pair<Float, Float>>{Containers::InPlaceInit, {{0.0f, 3.0f}}},
            linearInterpolator<Target, Float>};

        CORRADE_VERIFY(a.target() == Target::Weight);
        CORRADE_COMPARE(a.index(), 0);
        CORRADE_COMPARE(a.interpolator(), (linearInterpolator<Target, Float>));
        CORRADE_COMPARE(a.before(), Extrapolation::Constant);
        CORRADE_COMPARE(a.after(), Extrapolation::Constant);
        CORRADE_COMPARE(a.data().size(), 1);
    }
}

void TrackTest::constructInitializerList() {
    const Track<Vector3> a{Target::Translation, 5,
            {{0.0f, {3.0f, 1.0f, 0.1f}},
             {5.0f, {0.3f, 0.6f, 1.0f}}},
        constantInterpolator<Target, Float>,
        Extrapolation::Extrapolated, Extrapolation::DefaultConstructed};

    CORRADE_VERIFY(a.target() == Target::Translation);
    CORRADE_COMPARE(a.index(), 5);
    CORRADE_COMPARE(a.interpolator(), (constantInterpolator<Target, Float>));
    CORRADE_COMPARE(a.before(), Extrapolation::Extrapolated);
    CORRADE_COMPARE(a.after(), Extrapolation::DefaultConstructed);
    CORRADE_COMPARE(a.data().size(), 2);
    CORRADE_COMPARE(a.data()[1].first, 5.0f);
}

void TrackTest::constructInitializerListDefaults() {
    {
        const Track<Vector3> a{Target::Translation, 5, {{0.0f, {3.0f, 1.0f, 0.1f}}},
            linearInterpolator<Target, Float>};

        CORRADE_VERIFY(a.target() == Target::Translation);
        CORRADE_COMPARE(a.index(), 5);
        CORRADE_COMPARE(a.interpolator(), (linearInterpolator<Target, Float>));
        CORRADE_COMPARE(a.before(), Extrapolation::Constant);
        CORRADE_COMPARE(a.after(), Extrapolation::Constant);
        CORRADE_COMPARE(a.data().size(), 1);
    } {
        const Track<Float> a{Target::Weight, {{0.0f, 1.0f}},
            constantInterpolator<Target, Float>, Extrapolation::Extrapolated, Extrapolation::DefaultConstructed};

        CORRADE_VERIFY(a.target() == Target::Weight);
        CORRADE_COMPARE(a.index(), 0);
        CORRADE_COMPARE(a.interpolator(), (constantInterpolator<Target, Float>));
        CORRADE_COMPARE(a.before(), Extrapolation::Extrapolated);
        CORRADE_COMPARE(a.after(), Extrapolation::DefaultConstructed);
        CORRADE_COMPARE(a.data().size(), 1);
    } {
        const Track<Color3> a{Target::Color, {{0.0f, 0x0000ff_rgbf}},
            linearInterpolator<Target, Float>, Extrapolation::Extrapolated};

        CORRADE_VERIFY(a.target() == Target::Translation);
        CORRADE_COMPARE(a.index(), 0);
        CORRADE_COMPARE(a.interpolator(), (linearInterpolator<Target, Float>));
        CORRADE_COMPARE(a.before(), Extrapolation::Extrapolated);
        CORRADE_COMPARE(a.after(), Extrapolation::Extrapolated);
        CORRADE_COMPARE(a.data().size(), 1);
    }
}

void TrackTest::constructView() {
    const std::pair<Float, Vector3> data[]{
        {0.0f, {3.0f, 1.0f, 0.1f}},
        {5.0f, {0.3f, 0.6f, 1.0f}}};

    const TrackView<Vector3> a{Target::Translation, 5, data,
        constantInterpolator<Target, Float>,
        Extrapolation::Extrapolated, Extrapolation::DefaultConstructed};

    CORRADE_VERIFY(a.target() == Target::Translation);
    CORRADE_COMPARE(a.index(), 5);
    CORRADE_COMPARE(a.interpolator(), (constantInterpolator<Target, Float>));
    CORRADE_COMPARE(a.before(), Extrapolation::Extrapolated);
    CORRADE_COMPARE(a.after(), Extrapolation::DefaultConstructed);
    CORRADE_COMPARE(a.data().size(), 2);
    CORRADE_COMPARE(a.data()[1].first, 5.0f);
}

void TrackTest::constructViewDefaults() {
    {
        const std::pair<Float, Color3> data[]{
            {0.0f, 0xfafebf_rgbf}};

        const TrackView<Color3> a{Target::Color, 5, data,
            linearInterpolator<Target, Float>, Extrapolation::Extrapolated};

        CORRADE_VERIFY(a.target() == Target::Translation);
        CORRADE_COMPARE(a.index(), 5);
        CORRADE_COMPARE(a.interpolator(), (linearInterpolator<Target, Float>));
        CORRADE_COMPARE(a.before(), Extrapolation::Extrapolated);
        CORRADE_COMPARE(a.after(), Extrapolation::Extrapolated);
        CORRADE_COMPARE(a.data().size(), 1);
    } {
        const std::pair<Float, Vector3> data[]{
            {0.0f, {3.0f, 1.0f, 0.1f}}};

        const TrackView<Vector3> a{Target::Translation, data,
            constantInterpolator<Target, Float>, Extrapolation::Extrapolated, Extrapolation::DefaultConstructed};

        CORRADE_VERIFY(a.target() == Target::Translation);
        CORRADE_COMPARE(a.index(), 0);
        CORRADE_COMPARE(a.interpolator(), (constantInterpolator<Target, Float>));
        CORRADE_COMPARE(a.before(), Extrapolation::Extrapolated);
        CORRADE_COMPARE(a.after(), Extrapolation::DefaultConstructed);
        CORRADE_COMPARE(a.data().size(), 1);
    } {
        const std::pair<Float, Float> data[]{
            {0.0f, 3.0f}};

        const TrackView<Float> a{Target::Weight, data,
            linearInterpolator<Target, Float>, Extrapolation::Extrapolated};

        CORRADE_VERIFY(a.target() == Target::Translation);
        CORRADE_COMPARE(a.index(), 0);
        CORRADE_COMPARE(a.interpolator(), (linearInterpolator<Target, Float>));
        CORRADE_COMPARE(a.before(), Extrapolation::Extrapolated);
        CORRADE_COMPARE(a.after(), Extrapolation::Extrapolated);
        CORRADE_COMPARE(a.data().size(), 1);
    }
}

void TrackTest::convertView() {
    const Track<Vector3> a{Target::Translation, 5,
        {{0.0f, {3.0f, 1.0f, 0.1f}},
         {5.0f, {0.3f, 0.6f, 1.0f}}},
        constantInterpolator<Target, Float>,
        Extrapolation::Extrapolated, Extrapolation::DefaultConstructed};
    const TrackView<Vector3> av = a;

    CORRADE_VERIFY(av.target() == Target::Translation);
    CORRADE_COMPARE(av.index(), 5);
    CORRADE_COMPARE(av.interpolator(), (constantInterpolator<Target, Float>));
    CORRADE_COMPARE(av.before(), Extrapolation::Extrapolated);
    CORRADE_COMPARE(av.after(), Extrapolation::DefaultConstructed);
    CORRADE_COMPARE(av.data().size(), 2);
    CORRADE_COMPARE(av.data()[1].first, 5.0f);
}

void TrackTest::at() {
    setTestCaseDescription(AtData[testCaseInstanceId()].name);

    const Track<Float> a{Target::Weight, 5,
        {{0.0f, 3.0f},
         {2.0f, 1.0f},
         {4.0f, 2.5f},
         {5.0f, 0.5f}},
        constantInterpolator<Target, Float>,
        AtData[testCaseInstanceId()].extrapolationBefore, AtData[testCaseInstanceId()].extrapolationAfter};

    std::size_t hint{};
    CORRADE_COMPARE(a.at(AtData[testCaseInstanceId()].time, hint), AtData[testCaseInstanceId()].expectedValue);
    CORRADE_COMPARE(hint, AtData[testCaseInstanceId()].expectedHint);
}

void TrackTest::atSingleKeyframe() {
    setTestCaseDescription(AtSingleKeyframeData[testCaseInstanceId()].name);

    CORRADE_VERIFY(false);
}

void TrackTest::atNoKeyframe() {
    CORRADE_VERIFY(false);
}

void TrackTest::atHint() {
    setTestCaseDescription(AtHintData[testCaseInstanceId()].name);

    CORRADE_VERIFY(false);
}

void TrackTest::debugExtrapolation() {
    std::ostringstream out;

    Debug(&out) << Extrapolation::DefaultConstructed << Extrapolation(0xdead);
    CORRADE_COMPARE(out.str(), "Animation::Extrapolation::DefaultConstructed Animation::Extrapolation(0xdead)\n");
}

//     using namespace Math::Literals;
//
//     Float time{};
//
//     Trade::AnimationTrack<Vector3> characterFallTranslation{Trade::AnimationTarget::Translation, {
//         {0.0f, Vector3::yAxis(10.0f)},
//         {3.0f, Vector3::yAxis(5.0f)},
//         {3.8f, Vector3::yAxis(1.0f)},
//         {4.0f, Vector3::yAxis(0.0f)}},
//         linearInterpolator<Trade::AnimationTarget, Float>};
//
//     Trade::AnimationTrack<Quaternion> characterFallRotation{Trade::AnimationTarget::Rotation, {
//         {0.0f, Quaternion::rotation(0.0_degf, Vector3::zAxis())},
//         {4.0f, Quaternion::rotation(100.0_degf, Vector3::zAxis())}},
//         linearInterpolator<Trade::AnimationTarget, Float>};
//
//     Trade::AnimationTrack<Color4> characterFallColoring{Trade::AnimationTarget::Color, {
//         {0.0f, 0xffffff_srgbaf},
//         {3.8f, 0xffffff_srgbaf},
//         {4.0f, 0x800000_srgbaf}},
//         linearInterpolator<Trade::AnimationTarget, Float>};
//
//     Trade::AnimationClip characterFall{characterFallTranslation, characterFallRotation};
//     Trade::AnimationClip bloodColoring{characterFallColoring};
//
//     Animation::Animator<Trade::AnimationClip> animator;
//     animator.add(characterFall);
//     animator.add(bloodColoring);
//
//     animator.play(0, time);
//     animator.play(1, time + characterFall.end());
//
//     Color4 color = animator.advance<Trade::AnimationTarget::Color>(time);
//
//     Containers::ArrayView<std::tuple<Vector3, Quaternion>> boneTransformations;
//     animator.advanceIndexed<Trade::AnimationTarget::Translation,
//                             Trade::AnimationTarget::Rotation>(time, boneTransformations);
//
//
//
//
//
//
//     static_cast<void>(color);

}}}

CORRADE_TEST_MAIN(Magnum::Animation::Test::TrackTest)
