#pragma once

#include <string>

// 3D vector (often used to store floating-point colors)
struct ImVec3
{
    float     x, y, z;
    ImVec3() { x = y = z = 0.0f; }
    ImVec3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
    ImVec3(const Core::Maths::Vec3& f) { x = f.x; y = f.y; z = f.z; }
    operator Core::Maths::Vec3() const { return Core::Maths::Vec3(x, y, z); }
};

namespace ImGui
{
    IMGUI_API void PushItemToCenter(float width_percentage);
    IMGUI_API void PushItemToRight(const char* label = (const char*)0, float offset = 120.f);

    IMGUI_API bool RCheckbox(const std::string& label, bool* v);
    IMGUI_API bool RDragInt(const std::string& label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
    IMGUI_API bool RDragFloat(const std::string& label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool RDragFloat2(const std::string& label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool RDragFloat3(const std::string& label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool RDragFloat4(const std::string& label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool RColorEdit4(const std::string& label, float col[4], ImGuiColorEditFlags flags = 0);

    IMGUI_API bool RBeginCombo(const std::string& label, const char* preview_value, ImGuiComboFlags flags = 0);

    IMGUI_API bool ColoredButton(const char* label, const ImVec2& size_arg, const ImVec3& color);
    IMGUI_API void ImageUV(ImTextureID user_texture_id, const ImVec2& size);
    IMGUI_API bool ImageButtonUV_HelpMarker(ImTextureID user_texture_id, const char* help_marker, const ImVec2& size = { 16 });
    IMGUI_API bool ImageButtonUV(ImTextureID user_texture_id, const ImVec2& size = { 16 });
    IMGUI_API bool BeginComboButton(const char* label);
    IMGUI_API bool BeginComboColoredButton(const char* label, const ImVec3& color);
}
