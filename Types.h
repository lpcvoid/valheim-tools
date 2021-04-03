//
// Created by lpcvoid on 03/04/2021.
//

#pragma once

namespace valheim {

    template <typename T>
    using Vec2 = std::tuple<T,T>;

    template <typename T>
    using Vec3 = std::tuple<T,T,T>;

    using Position = std::tuple<float, float, float>;
    using Quaternion = std::tuple<float, float, float, float>;
}

