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
        PushItemWidth(GetContentRegionAvail().x);
    }

    IMGUI_API bool RCheckbox(const std::string& label, bool* v)
    {
        PushItemToRight(label.c_str());
        return Checkbox(("##" + label).c_str(), v);
    }

    IMGUI_API bool RDragInt(const std::string& label, int* v, float v_speed, int v_min, int v_max, const char* format)
    {
        Text(label.c_str());
        SameLine(GetContentRegionAvail().x / 3);
        PushItemWidth(GetContentRegionAvail().x);
        return DragInt(("##" + label).c_str(), v, v_speed, v_min, v_max, format);
    }

    IMGUI_API bool RDragFloat(const std::string& label, float* v, float v_speed, float v_min, float v_max, const char* format, float power)
    {
        PushItemToRight(label.c_str());
        return DragFloat(("##" + label).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    IMGUI_API bool RDragFloat2(const std::string& label, float v[2], float v_speed, float v_min, float v_max, const char* format, float power)
    {
        PushItemToRight(label.c_str());
        return DragFloat2(("##" + label).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    IMGUI_API bool RDragFloat3(const std::string& label, float v[3], float v_speed, float v_min,float v_max, const char* format, float power)
    {
        PushItemToRight(label.c_str());
        return DragFloat3(("##" + label).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    IMGUI_API bool RDragFloat4(const std::string& label, float v[4], float v_speed, float v_min, float v_max, const char* format, float power)
    {
        PushItemToRight(label.c_str());
        return DragFloat4(("##" + label).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    IMGUI_API bool RColorEdit4(const std::string& label, float col[4], ImGuiColorEditFlags flags)
    {
        PushItemToRight(label.c_str());
        return ColorEdit4(("##" + label).c_str(), col, flags);
    }

    IMGUI_API bool RBeginCombo(const std::string& label, const char* preview_value, ImGuiComboFlags flags)
    {
        PushItemToRight(label.c_str());
        return BeginCombo(("##" + label).c_str(), preview_value, flags);
    }

    IMGUI_API bool ColoredButton(const char* label, const ImVec2& size_arg, const ImVec3& color)
    {
        PushStyleColor(ImGuiCol_Button, { color.x, color.y, color.z, 1.0f });
        PushStyleColor(ImGuiCol_ButtonHovered, { color.x, color.y, color.z, 0.7f });
        bool isPressed = Button(label, size_arg);
        PopStyleColor(2);

        return isPressed;
    }

    IMGUI_API void ImageUV(ImTextureID user_texture_id, const ImVec2& size)
    {
        return Image(user_texture_id, size, { 0.f, 1.f }, { 1.f, 0.f });
    }

    IMGUI_API bool ImageButtonUV_HelpMarker(ImTextureID user_texture_id, const char* help_marker, const ImVec2& size)
    {
        bool isClicked = ImageButton(user_texture_id, size, { 0.f, 1.f }, { 1.f, 0.f });
        HelpMarkerItem(help_marker);

        return isClicked;
    }

    IMGUI_API bool ImageButtonUV(ImTextureID user_texture_id, const ImVec2& size)
    {
        return ImageButton(user_texture_id, size, { 0.f, 1.f }, { 1.f, 0.f });
    }

    IMGUI_API bool ImageButtonUVWithText_HelpMarker(ImTextureID user_texture_id, const char* label, const std::string& text, const char* help_marker, const ImVec2& icon_size)
    {
        bool isClicked = ImageButtonUVWithText(user_texture_id, label, text, icon_size);
        HelpMarkerItem(help_marker);

        return isClicked;
    }

    IMGUI_API bool ImageButtonUVWithText(ImTextureID user_texture_id, const char* label, const std::string& text, const ImVec2& icon_size)
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

        window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, { 0.f, 1.f }, { 1.f, 0.f });

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
                popup_window->AutoPosLastDirection = ImGuiDir_Left;
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

    IMGUI_API bool BeginComboColoredButton(const char* label, const ImVec3& color)
    {
        PushStyleColor(ImGuiCol_Button, { color.x, color.y, color.z, 1.0f });
        PushStyleColor(ImGuiCol_ButtonHovered, { color.x, color.y, color.z, 0.7f });
        bool isOpen = BeginComboButton(label);
        PopStyleColor(2);

        return isOpen;
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
