#include "Material.h"

Color Material::getLighting(Normal &N, const Color &LightColor, const Point3 &LightDir, const Point3 &ViewVector) {
    float m_ambient = 0.1;
    auto lightDir = LightDir.normal();
    Color ambient = ambientColor * m_ambient * baseColor;
    auto viewDir = -ViewVector.normal();

    auto I_d = kd*baseColor*(dot(N, lightDir));

    auto R = 2*(dot(lightDir, N))*N - lightDir;
    auto I_s = ks*specColor*std::pow((dot(viewDir, R)),kgls);

    //Clip the values
    auto light_total = I_d + I_s + ambient;
    light_total = clip(light_total, 0, 255);
    light_total = light_total + ambient;
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
