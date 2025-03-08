#include "Material.h"

Color Material::getLighting(Normal &N, const Color &LightColor, const Point3 &LightDir, const Vect3<float> &RayDir) {
    float m_ambient = 0.1;
    auto lightDir = LightDir.normal();
    Color ambient = ambientColor * m_ambient * baseColor;
    auto viewDir = -RayDir.normal();

    auto I_d = kd*baseColor*(dot(N, lightDir));

    auto R = (N * (2.0 * dot(N, lightDir))) - lightDir;
    auto I_s = ks*specColor*LightColor*std::pow(std::max(0.0f,(dot(viewDir, R))),kgls);

    //Clip the values
    auto light_total = I_d + I_s + ambient;
    light_total = clip(light_total, 0, 255);
    return light_total;
}

void Material::setBaseColor(Color new_color) {
    baseColor = new_color;
}

void Material::setSpecColor(Color new_color){
    specColor = new_color;
}

Color Material::clip(Color nVect, float lower, float upper) {
    auto newN = Color(0, 0, 0);
    for (int i = 0; i < 3; i++) {
        newN.v[i] = std::max(lower, std::min((nVect.v[i]), upper));
    }
    return newN;
}
