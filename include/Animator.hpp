#pragma once
#include <functional>

namespace games {

    template<typename T>
    class Animation {
    public:
        using SetterFunction = std::function<void(const T&)>;
        using LerpFunction = std::function<T(const T&, const T&, float)>;

        void animate(const T& startValue, const T& endValue, float duration, SetterFunction setter,
                LerpFunction lerpFunc = [](const T& start, const T& end, float t) {
                    return start + t * (end - start);
                }) {
            animations.emplace_back(startValue, endValue, duration, setter, lerpFunc);
        }

        void update(float deltaTime) {
            for (auto& animation : animations) {
                animation.currentDuration += deltaTime;
                float t = std::min(animation.currentDuration / animation.duration, 1.0f);
                T interpolatedValue = animation.lerpFunc(animation.startValue, animation.endValue, t);
                animation.setter(interpolatedValue);
            }
        }

        void clear() {
            animations.clear();
        }

    private:
        struct AnimationData {
            T startValue;
            T endValue;
            float duration;
            float currentDuration = 0.0f;
            SetterFunction setter;
            LerpFunction lerpFunc;

            AnimationData(const T& start, const T& end, float dur, SetterFunction set,
                std::function<T(const T&, const T&, float)> lerp)
                : startValue(start), endValue(end), duration(dur), setter(set), lerpFunc(lerp) {}
        };

        std::vector<AnimationData> animations;
    };

}