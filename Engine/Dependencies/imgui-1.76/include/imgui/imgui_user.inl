#include <string>
#include <filesystem>

#include "imgui_user.h"
#include "Maths/Vec3.hpp"

namespace ImGui
{
    void PushItemToRight(const char* label, float offset)
    {
        if (label)
            Text(label);
        SameLine(offset);
        PushItemWidth(-FLT_MIN);
    }

    bool RCheckbox(const char* label, bool* v)
    {
        PushItemToRight(label);
        return Checkbox(("##" + std::string(label)).c_str(), v);
    }

    bool RDragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format)
    {
        PushItemToRight(label);
        return DragInt(("##" + std::string(label)).c_str(), v, v_speed, v_min, v_max, format);
    }

    bool RDragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, float power)
    {
        PushItemToRight(label);
        return DragFloat(("##" + std::string(label)).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    bool RDragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* format, float power)
    {
        PushItemToRight(label);
        return DragFloat2(("##" + std::string(label)).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    bool RDragFloat3(const char* label, float v[3], float v_speed, float v_min,float v_max, const char* format, float power)
    {
        PushItemToRight(label);
        return DragFloat3(("##" + std::string(label)).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    bool RDragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* format, float power)
    {
        PushItemToRight(label);
        return DragFloat4(("##" + std::string(label)).c_str(), v, v_speed, v_min, v_max, format, power);
    }

    bool RColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
    {
        PushItemToRight(label);
        return ColorEdit4(("##" + std::string(label)).c_str(), col, flags);
    }

    bool RBeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
    {
        PushItemToRight(label);
        return BeginCombo(("##" + std::string(label)).c_str(), preview_value, flags);
    }

    void RButtonDD(const char* label, const char* value)
    {
        PushItemToRight(label);
        PushStyleColor(ImGuiCol_Button, GImGui->Style.Colors[ImGuiCol_FrameBg]);
        PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

        ButtonEx(value, { ImGui::GetContentRegionAvail().x, 0.f }, ImGuiButtonFlags_Disabled);

        PopStyleVar();
        PopStyleColor();
    }

    void ImageUV(unsigned int user_texture_id, const ImVec2& size)
    {
#pragma warning(suppress : 4312)
        return Image((ImTextureID)user_texture_id, size, { 0.f, 1.f }, { 1.f, 0.f });
    }

    void ImageUVFramePadding(unsigned int user_texture_id, const ImVec2& size)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImVec2 padding = style.FramePadding;
        ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
        ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);

        ItemSize(bb);
        if (!ItemAdd(bb, 0))
            return;

#pragma warning(suppress : 4312)
        window->DrawList->AddImage((ImTextureID)user_texture_id, image_bb.Min, image_bb.Max, { 0.f, 1.f }, { 1.f, 0.f }, GetColorU32({ 1.f, 1.f, 1.f, 1.f }));
    }

    bool ImageButtonUV(unsigned int user_texture_id, const ImVec2& size)
    {
#pragma warning(suppress : 4312)
        return ImageButton((ImTextureID)user_texture_id, size, { 0.f, 1.f }, { 1.f, 0.f });
    }

    bool ImageButtonUVWithText(unsigned int user_texture_id, const char* label, const ImVec2& icon_size)
    {
        const char* button_text = label;
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
        RenderNavHighlight(bb, id);
        RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));

#pragma warning(suppress : 4312)
        window->DrawList->AddImage((ImTextureID)user_texture_id, image_bb.Min, image_bb.Max, { 0.f, 1.f }, { 1.f, 0.f }, GetColorU32({ 1.f, 1.f, 1.f, 1.f }));

        if (textSize.x > 0)
            ImGui::RenderText(start, button_text);

        return pressed;
    }

    bool CollapsingHeaderWithImageUV(unsigned int user_texture_id, const char* label, ImGuiTreeNodeFlags flags)
    {
        bool isOpen = ImGui::CollapsingHeader(("##" + std::string(label)).c_str(), flags);
        ImGui::SameLine(25.f + GImGui->Style.FramePadding.x);
        ImGui::ImageUVFramePadding(user_texture_id);
        ImGui::SameLine();
        ImGui::Text(label);

        return isOpen;
    }

    static float CalcMaxPopupHeightFromItemCount(int items_count)
    {
        ImGuiContext& g = *GImGui;
        if (items_count <= 0)
            return FLT_MAX;
        return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
    }

    bool BeginComboButton(const char* label, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
        const float w = size.x;

        const ImRect bb(pos, pos + size);
        ItemSize(size, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held);

        const ImU32 frame_col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

        RenderNavHighlight(bb, id);
        RenderFrame(bb.Min, bb.Max, frame_col, true, style.FrameRounding);
        RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
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
                ImVec2 pos = FindBestWindowPosForPopupEx(bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, bb, ImGuiPopupPositionPolicy_ComboBox);
                SetNextWindowPos(pos);
            }

        // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;

        // Horizontally align ourselves with the framed text
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.FramePadding.x * 2.f, style.WindowPadding.y));
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

    void HelpMarkerItem(const char* help_marker)
    {
        if (help_marker != NULL && ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(help_marker);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    void ImGui::StyleColorsBakerDark(ImGuiStyle* dst)
    {
        ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.55f, 0.55f, 0.58f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.55f, 0.58f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.50f, 1.00f, 0.50f, 0.33f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }

    void ImGui::StyleColorsBakerLight(ImGuiStyle* dst)
    {
        ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        colors[ImGuiCol_Text] = ImVec4(0.00f, 0.13f, 0.32f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
        colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.76f, 0.80f, 0.84f, 0.93f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.60f, 0.73f, 0.88f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.83f, 0.89f, 0.97f, 0.99f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.74f, 0.82f, 0.91f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.22f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.78f, 0.87f, 0.98f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.07f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    }
}
