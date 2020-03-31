#include "Maths/Vec3.hpp"
#include "imgui_user.h"

namespace ImGui
{
	IMGUI_API bool ColoredButton(const char* label, const ImVec2& size_arg, const ImVec3& color)
	{
		PushStyleColor(ImGuiCol_Button, { color.x, color.y, color.z, 1.0f });
		PushStyleColor(ImGuiCol_ButtonHovered, { color.x, color.y, color.z, 0.7f });
		bool isPressed = Button(label, size_arg);
		PopStyleColor(2);

		return isPressed;
	}

	IMGUI_API bool RDragFloat3(const std::string& label, float v[3], float v_speed, float v_min, float v_max, const char* format, float power)
	{
		Text(label.c_str());
		SameLine(GetContentRegionAvail().x / 3);
		PushItemWidth(GetContentRegionAvail().x);
		return DragFloat3(("##" + label).c_str(), v, v_speed, v_min, v_max, format, power);
	}

	IMGUI_API void ImageUV(ImTextureID user_texture_id, const ImVec2& size)
	{
		return Image(user_texture_id, size, { 0.f, 1.f }, { 1.f, 0.f });
	}

	IMGUI_API bool ImageButtonUV(ImTextureID user_texture_id, const ImVec2& size)
	{
		return ImageButton(user_texture_id, size, { 0.f, 1.f }, { 1.f, 0.f });
	}
}