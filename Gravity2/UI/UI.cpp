#include "stdafx.h"


WindowIOEvent::WindowIOEvent() : enabled(true), onEvent(false) {}


WindowIOEvent::operator bool() {
	if (!enabled)
		return false;

	return onEvent;
}


GravityWindow::GravityWindow() :
	isActive(false), imguiWindow(nullptr), application(nullptr), keyEvent{}, name() {}


GravityWindow::GravityWindow(const GravityWindow &Other) { *this = Other; }


//GravityWindow::GravityWindow(GravityWindow &&Other) { *this = std::move(Other); }


GravityWindow& GravityWindow::operator= (const GravityWindow &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		isActive	= Other.isActive;
		imguiWindow = Other.imguiWindow;
		application = Other.application;
		name		= Other.name;
	}

	return *this;
}


//GravityWindow& GravityWindow::operator= (GravityWindow &&Other) {
//	_ASSERTE(this != &Other);
//
//	if (this != &Other) {
//		Release();
//
//		isActive	= Other.isActive;
//		imguiWindow = Other.imguiWindow;
//		application = Other.application;
//
//		Other.Release();
//	}
//
//	return *this;
//}


GravityWindow::~GravityWindow() { Release(); }


void GravityWindow::Init(const String &Name, GravityApp *&Application) {
	name = Name;
	application = Application;
}


void GravityWindow::Release() {
	isActive		= false;
	imguiWindow		= nullptr;
	application		= nullptr;
}


void GravityWindow::Draw() {}


void GravityWindow::Events() {}


void ImGuiCustomToolTip(const char *Description, float WrapMultiplier) {
	ImGui::SameLine();
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * WrapMultiplier);
		ImGui::TextWrapped(Description);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::NewLine();
}

void ImGuiCustomColumnPairToolTip(Pair<const char*, const char*> *Params, size_t Length, float WrapMultiplier) {
	ImGui::SameLine();
	if (ImGui::IsItemHovered()) {
		float keyWidth		= 0.0f;
		float valueWidth	= 0.0f;
		float popupHeight	= ImGui::GetTextLineHeightWithSpacing() * Length;
		float wrapWidth		= ImGui::GetFontSize() * WrapMultiplier;
		ImVec2 textSize{};
		// Get longest name within the params key and set is as the base length;
		for (size_t i = 0; i < Length; i++) {
			textSize = ImGui::CalcTextSize(Params[i].Key);
			if (keyWidth < textSize.x)
				keyWidth = textSize.x;

			textSize = ImGui::CalcTextSize(Params[i].Value, nullptr, false, wrapWidth);
			if (valueWidth < textSize.x)
				valueWidth = textSize.x;

			if (textSize.y > ImGui::GetTextLineHeightWithSpacing())
				popupHeight += textSize.y * 0.5f;
		}
		// Add some padding.
		keyWidth	+= 5.0f;
		valueWidth	+= 10.0f;

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.08f, 0.08f, 0.94f));
		ImGui::BeginTooltip();
		ImGui::BeginChild("##Col1", ImVec2(keyWidth, popupHeight));
		for (size_t i = 0; i < Length; i++)
			ImGui::Text(Params[i].Key);
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##Col2", ImVec2(valueWidth, popupHeight));
		ImGui::PushTextWrapPos(wrapWidth);
		for (size_t i = 0; i < Length; i++) {
			ImGui::Text(":");
			ImGui::SameLine(10.0f);
			ImGui::TextWrapped(Params[i].Value);
		}
		ImGui::PopTextWrapPos();
		ImGui::EndChild();
		ImGui::EndTooltip();
		ImGui::PopStyleColor();
	}
	ImGui::NewLine();
}


ImGuiWindow *WindowDebugger::window = nullptr;
ImVec4 WindowDebugger::colour = {};


void WindowDebugger::RenderDebugger() {
	static float refColour[4];
	ImGui::Begin("ImGui Debugger");
	if (window) {
		ImGui::Text("Name: %s", window->Name);
		ImGui::Text("ID: %d", window->ID);
		ImGui::Text("Size X: %.2f", window->Size.x);
		ImGui::Text("Size Y: %.2f", window->Size.y);
	}
	ImGui::ColorPicker4("##", (float*)&colour, ImGuiColorEditFlags_Float, refColour);
	ImGui::End();
}