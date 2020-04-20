#include <string>
#include <filesystem>

#include "imgui_user.h"
#include "Maths/Vec3.hpp"

namespace ImGui
{
    IMGUI_API void PushItemToCenter(float width_percentage)
    {
        ImGui::Indent(GetWindowContentRegionWidth() * ((1 - width_percentage) / 2));
        ImGui::SetNextItemWidth(GetWindowContentRegionWidth() * width_percentage);
    }

    IMGUI_API void PushItemToRight(const char* label, float offset)
    {
        if (label)
            Text(label);
        SameLine(offset);
        PushItemWidth(-FLT_MIN);
    }

    IMGUI_API bool RCheckbox(const char* label, bool* v)
    {
        PushItemToRight(label);
        return Checkbox(("##" + std::string(label)).c_str(), v);
    }

    IMGUI_API bool RDragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format)
    {
        PushItemToRight(label);
        return DragInt(("##" + std::string(label)).c_str(), v, v_speed, v_min, v_max, format);
    }

    IMGUI_API bool RDragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, float power)
    {
        PushItemToRight(label);
        return DragFloat(("##" + std::string(label)).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    IMGUI_API bool RDragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* format, float power)
    {
        PushItemToRight(label);
        return DragFloat2(("##" + std::string(label)).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    IMGUI_API bool RDragFloat3(const char* label, float v[3], float v_speed, float v_min,float v_max, const char* format, float power)
    {
        PushItemToRight(label);
        return DragFloat3(("##" + std::string(label)).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    IMGUI_API bool RDragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* format, float power)
    {
        PushItemToRight(label);
        return DragFloat4(("##" + std::string(label)).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    IMGUI_API bool RColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
    {
        PushItemToRight(label);
        return ColorEdit4(("##" + std::string(label)).c_str(), col, flags);
    }

    IMGUI_API bool RBeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
    {
        PushItemToRight(label);
        return BeginCombo(("##" + std::string(label)).c_str(), preview_value, flags);
    }

    IMGUI_API void RButtonDD(const char* label, const char* value)
    {
        PushItemToRight(label);
        PushStyleColor(ImGuiCol_Button, GImGui->Style.Colors[ImGuiCol_FrameBg]);
        PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

        ButtonEx(value, { ImGui::GetContentRegionAvail().x, 0.f }, ImGuiButtonFlags_Disabled);

        PopStyleVar();
        PopStyleColor();
    }

    IMGUI_API void ImageUV(unsigned int user_texture_id, const ImVec2& size)
    {
#pragma warning(suppress : 4312)
        return Image((ImTextureID)user_texture_id, size, { 0.f, 1.f }, { 1.f, 0.f });
    }

    IMGUI_API bool ImageButtonUV(unsigned int user_texture_id, const ImVec2& size)
    {
#pragma warning(suppress : 4312)
        return ImageButton((ImTextureID)user_texture_id, size, { 0.f, 1.f }, { 1.f, 0.f });
    }

    IMGUI_API bool ImageButtonUVWithText(unsigned int user_texture_id, const char* label, const std::string& text, const ImVec2& icon_size)
    {
        const char* button_text = text.c_str();
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImVec2 size = icon_size;
        if (size.x <= 0 && size.y <= 0)
        {
            size.x = size.y = ImGui::GetTextLineHeightWithSpacing();
        }
        else
        {
            if (size.x <= 0)
                size.x = size.y;
            else if (size.y <= 0)
                size.y = size.x;
            size *= window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
        }

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        const ImGuiID id = window->GetID(label);
        const ImVec2 textSize = ImGui::CalcTextSize(button_text, NULL, true);

        const float innerSpacing = style.ItemInnerSpacing.x;
        const ImVec2 padding = style.FramePadding;
        const ImVec2 totalSizeWithoutPadding(size.x + innerSpacing + textSize.x, size.y > textSize.y ? size.y : textSize.y);
        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding * 2);

        ImVec2 start = window->DC.CursorPos + padding;
        if (size.y < textSize.y)
            start.y += (textSize.y - size.y) * 0.5f;

        const ImRect image_bb(start, start + size);
        start = window->DC.CursorPos + padding;
        start.x += size.x + innerSpacing;
        if (size.y > textSize.y)
            start.y += (size.y - textSize.y) * 0.5f;

        ItemSize(bb);
        if (!ItemAdd(bb, id))
            return false;

        bool hovered = false, held = false;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held);

        // Render
        const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));

#pragma warning(suppress : 4312)
        window->DrawList->AddImage((ImTextureID)user_texture_id, image_bb.Min, image_bb.Max, { 0.f, 1.f }, { 1.f, 0.f });

        if (textSize.x > 0)
            ImGui::RenderText(start, button_text);

        return pressed;
    }

    static float CalcMaxPopupHeightFromItemCount(int items_count)
    {
        ImGuiContext& g = *GImGui;
        if (items_count <= 0)
            return FLT_MAX;
        return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
    }

    IMGUI_API bool BeginComboButton(const char* label)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const float w = CalcItemWidth();
        const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
        const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id, &frame_bb))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(frame_bb, id, &hovered, &held);

        const ImU32 frame_col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

        RenderNavHighlight(frame_bb, id);
        RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);
        RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, label, NULL, NULL, style.ButtonTextAlign, &frame_bb);

        bool popup_open = IsPopupOpen(id);
        if ((pressed || g.NavActivateId == id) && !popup_open)
        {
            if (window->DC.NavLayerCurrent == 0)
                window->NavLastIds[0] = id;
            OpenPopupEx(id);
            popup_open = true;
        }

        if (!popup_open)
            return false;

        int popup_max_height_in_items = 4;
        SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

        char name[16];
        ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

        // Peak into expected window size so we can position it
        if (ImGuiWindow* popup_window = FindWindowByName(name))
            if (popup_window->WasActive)
            {
                ImVec2 size_expected = CalcWindowExpectedSize(popup_window);
                popup_window->AutoPosLastDirection = ImGuiDir_Right | ImGuiDir_Down;
                ImRect r_outer = GetWindowAllowedExtentRect(popup_window);
                ImVec2 pos = FindBestWindowPosForPopupEx(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, frame_bb, ImGuiPopupPositionPolicy_ComboBox);
                SetNextWindowPos(pos);
            }

        // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;

        // Horizontally align ourselves with the framed text
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
        bool ret = Begin(name, NULL, window_flags);
        PopStyleVar();

        if (!ret)
        {
            EndPopup();
            IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
            return false;
        }
        return true;
    }

    IMGUI_API bool BeginComboImageButtonUV(unsigned int user_texture_id, const ImVec2& size)
    {
        int frame_padding = -1;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        PushID((void*)(intptr_t)user_texture_id);
        const ImGuiID id = window->GetID("#image");
        PopID();

        const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
        const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
        const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
        ItemSize(frame_bb);
        if (!ItemAdd(frame_bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(frame_bb, id, &hovered, &held);

        const ImU32 frame_col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

        RenderNavHighlight(frame_bb, id);
        RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

#pragma warning(suppress : 4312)
        window->DrawList->AddImage((ImTextureID)user_texture_id, image_bb.Min, image_bb.Max, { 0.f, 1.f }, { 1.f, 0.f });

        bool popup_open = IsPopupOpen(id);
        if ((pressed || g.NavActivateId == id) && !popup_open)
        {
            if (window->DC.NavLayerCurrent == 0)
                window->NavLastIds[0] = id;
            OpenPopupEx(id);
            popup_open = true;
        }

        if (!popup_open)
            return false;

        int popup_max_height_in_items = 4;
        SetNextWindowSizeConstraints(ImVec2(size.x, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

        char name[16];
        ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

        // Peak into expected window size so we can position it
        if (ImGuiWindow* popup_window = FindWindowByName(name))
            if (popup_window->WasActive)
            {
                ImVec2 size_expected = CalcWindowExpectedSize(popup_window);
                popup_window->AutoPosLastDirection = ImGuiDir_Right | ImGuiDir_Down;
                ImRect r_outer = GetWindowAllowedExtentRect(popup_window);
                ImVec2 pos = FindBestWindowPosForPopupEx(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, frame_bb, ImGuiPopupPositionPolicy_ComboBox);
                SetNextWindowPos(pos);
            }

        // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;

        // Horizontally align ourselves with the framed text
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
        bool ret = Begin(name, NULL, window_flags);
        PopStyleVar();

        if (!ret)
        {
            EndPopup();
            IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
            return false;
        }
        return true;
    }

    IMGUI_API void HelpMarkerItem(const char* help_marker)
    {
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(help_marker);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }
}
