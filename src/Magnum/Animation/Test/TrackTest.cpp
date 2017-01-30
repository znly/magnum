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

#include <Corrade/TestSuite/Tester.h>

#include "Magnum/Animation/Animator.h" /** @todo remove */
#include "Magnum/Animation/Track.h"
#include "Magnum/Math/Color.h"
#include "Magnum/Math/Quaternion.h"
#include "Magnum/Trade/Animation.h" /** @todo also */

namespace Magnum { namespace Animation { namespace Test {

struct TrackTest: TestSuite::Tester {
    explicit TrackTest();

    void construct();

    void at();
    void atSingleKeyframe();

    void atHint();
};

namespace {

enum: std::size_t { AtDataCount = 8 };

const struct {
    const char* name;
    Extrapolation extrapolation;
    Float time;
    Float expectedValue;
} AtData[AtDataCount] {
    {"before default-constructed", Extrapolation::DefaultConstructed, 15.0f, 7.3f},
    {"before constant", Extrapolation::Constant, 15.0f, 7.3f},
    {"before extrapolated", Extrapolation::Extrapolated, 15.0f, 7.3f},
    {"during first", {}, 15.0f, 7.3f},
    {"during second", {}, 15.0f, 7.3f},
    {"after default-constructed", Extrapolation::DefaultConstructed, 15.0f, 7.3f},
    {"after constant", Extrapolation::Constant, 15.0f, 7.3f},
    {"after extrapolated", Extrapolation::Extrapolated, 15.0f, 7.3f}
};

}

TrackTest::TrackTest() {
    addTests({&TrackTest::construct});

    addInstancedTests({&TrackTest::at}, AtDataCount);

//     addInstancedTests({&TrackTest::atSingleKeyframe}, AtSingleKeyframeDataCount);
}

void TrackTest::construct() {

    Float time{};


    Trade::AnimationClip characterFall{{}};
    Trade::AnimationClip bloodColoring{{}};

    Animation::Animator<Trade::AnimationClip> animator;
    animator.add(characterFall);
    animator.add(bloodColoring);

    animator.play(0, time);
    animator.play(1, time + characterFall.end());

    Color4 color = animator.advance<Trade::AnimationTarget::Color>(time);

    Containers::ArrayView<std::tuple<Vector3, Quaternion, Vector3>> boneTransformations;
    animator.advanceIndexed<Trade::AnimationTarget::Translation,
                            Trade::AnimationTarget::Rotation,
                            Trade::AnimationTarget::Scaling>(time, boneTransformations);






    static_cast<void>(color);
}

}}}

CORRADE_TEST_MAIN(Magnum::Animation::Test::TrackTest)
