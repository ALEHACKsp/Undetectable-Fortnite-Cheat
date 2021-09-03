#include "sdk.h"
#define xorthis(str) _xor_(str).c_str()


#include <iostream>
#include <Windows.h>
#include <ios>
#include <limits>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <sstream>
#include <string>

#include <urlmon.h>
#include <tchar.h>

#include <string>
#include <conio.h>
#include <cstdlib>
#include <stdlib.h>
#include <ostream>
#include <regex>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include <dwmapi.h>
#pragma comment (lib, "urlmon.lib")
#include <iostream>
#include <Windows.h>

#include "d3d.h"
#include <dwmapi.h>
#include <vector>


#include <fstream>
#include <direct.h>


#include <string>
#include <conio.h>
#include <cstdlib>
#include <stdlib.h>
#include <ostream>
#include <regex>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <codecvt>
#include <fcntl.h>
#include <io.h>



#include <urlmon.h>
//#include "picosha2.h"
#include <tchar.h>
#include <sddl.h>
//#include "manager.h"
//#include <windows.h>
//#include "xor.h"


#include <strsafe.h>
//#include "hwid.h"
//#include "Crypto.h"
//#include "auth.h"


#include <cstdlib>

#include <Lmcons.h>



using namespace std;




//#include "AutoUpdate.h"
#include <filesystem>
//#include <examples\example_win32_directx11\MyMACAddr.h>

#include <tchar.h>

#include <cstring>
#include <atlstr.h>
#include <windef.h>
#include <sstream>

#include <TlHelp32.h>
#include <thread>


























void fn::keys()
{
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		settings.menu = !settings.menu;
	}
}

bool fn::actorloop(ImGuiWindow& window)
{
	uintptr_t MyTeamIndex;
	uintptr_t EnemyTeamIndex;
	float FOVmax = 9999.f;

	X = SDK::Utilities::SpoofCall(GetSystemMetrics, SM_CXSCREEN);
	Y = SDK::Utilities::SpoofCall(GetSystemMetrics, SM_CYSCREEN);

	uintptr_t GWorld = SDK::Utilities::read<uintptr_t>(Details.UWORLD);
	if (!GWorld) return false;

	uintptr_t Gameinstance = SDK::Utilities::read<uint64_t>(GWorld + SDK::Classes::StaticOffsets::OwningGameInstance);
	if (!Gameinstance) return false;

	uintptr_t LocalPlayer = SDK::Utilities::read<uint64_t>(Gameinstance + SDK::Classes::StaticOffsets::LocalPlayers);
	if (!LocalPlayer) return false;

	uintptr_t LocalPlayers = SDK::Utilities::read<uint64_t>(LocalPlayer);
	if (!LocalPlayers) return false;

	PlayerController = SDK::Utilities::read<uint64_t>(LocalPlayers + SDK::Classes::StaticOffsets::PlayerController);
	if (!PlayerController) return false;

	PlayerCameraManager = SDK::Utilities::read<uintptr_t>(PlayerController + SDK::Classes::StaticOffsets::PlayerCameraManager);
	if (!PlayerCameraManager) return false;

	LocalPawn = SDK::Utilities::read<uint64_t>(PlayerController + SDK::Classes::StaticOffsets::AcknowledgedPawn);
	if (!LocalPawn) return false;

	uintptr_t Ulevel = SDK::Utilities::read<uintptr_t>(GWorld + SDK::Classes::StaticOffsets::PersistentLevel);
	if (!Ulevel) return false;

	uintptr_t AActors = SDK::Utilities::read<uintptr_t>(Ulevel + SDK::Classes::StaticOffsets::AActors);
	if (!AActors) return false;

	uintptr_t ActorCount = SDK::Utilities::read<int>(Ulevel + SDK::Classes::StaticOffsets::ActorCount);
	if (!ActorCount) return false;

	GetPlayerViewPoint(PlayerCameraManager, &SDK::Utilities::CamLoc, &SDK::Utilities::CamRot);

	for (int i = 0; i < ActorCount; i++) {

		uintptr_t CurrentActor = SDK::Utilities::read<uint64_t>(AActors + i * sizeof(uintptr_t));

		std::string NameCurrentActor = GetObjectName(CurrentActor);
		

		if (strstr(NameCurrentActor.c_str(), xorthis("PlayerPawn_")) || strstr(NameCurrentActor.c_str(), xorthis("PlayerPawn_Athena_Phoebe_C")) || strstr(NameCurrentActor.c_str(), xorthis("WeakSpot")))
		{
			uintptr_t MyState = SDK::Utilities::read<uintptr_t>(LocalPawn + SDK::Classes::StaticOffsets::PlayerState);
			if (!MyState) continue;

			MyTeamIndex = SDK::Utilities::read<uintptr_t>(MyState + SDK::Classes::StaticOffsets::TeamIndex);
			if (!MyTeamIndex) continue;

			uintptr_t EnemyState = SDK::Utilities::read<uintptr_t>(CurrentActor + SDK::Classes::StaticOffsets::PlayerState);
			if (!EnemyState) continue;

			EnemyTeamIndex = SDK::Utilities::read<uintptr_t>(EnemyState + SDK::Classes::StaticOffsets::TeamIndex);
			if (!EnemyTeamIndex) continue;

			SDK::Structs::Vector3 Headbox, bottom, pelviss;

			SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 66, &Headbox);
			SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 2, &pelviss);
			SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 0, &bottom);

			SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(Headbox.x, Headbox.y, Headbox.z + 20), &Headbox);
			SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(pelviss.x, pelviss.y, pelviss.z + 70), &pelviss);
			SDK::Classes::AController::WorldToScreen(bottom, &bottom);

			if (Headbox.x == 0 && Headbox.y == 0) continue;
			if (bottom.x == 0 && bottom.y == 0) continue;
			if (MyTeamIndex == EnemyTeamIndex) continue;
			if (CurrentActor == LocalPawn) continue;

			float Height = Headbox.y - bottom.y;
			if (Height < 0)
				Height = Height * (-1.f);
			float Width = Height * 0.65;
			Headbox.x = Headbox.x - (Width / 2);

			float Height1 = Headbox.y - bottom.y;

			if (Height1 < 0)
				Height1 = Height1 * (-1.f);
			float Width1 = Height1 * 0.35;

			SDK::Structs::Vector3 head2, neck, pelvis, chest, leftShoulder, rightShoulder, leftElbow, rightElbow, leftHand, rightHand, leftLeg, rightLeg, leftThigh, rightThigh, leftFoot, rightFoot, leftFeet, rightFeet, leftFeetFinger, rightFeetFinger;

			SDK::Structs::Vector3 viewPoint;
			bool IsVisible;
			ImColor col = { 255, 255, 255, 220 };
			ImColor SkelColor = { 255, 255, 255, 220};
			if (settings.vischeck) {
				IsVisible = SDK::Classes::APlayerCameraManager::LineOfSightTo((PVOID)PlayerController, (PVOID)CurrentActor, &viewPoint);
				if (IsVisible) {
					col = { 25, 255, 25 };
				}
				else {
					col = { 255, 25, 25 };
				}
				if (IsVisible)
				{
					SkelColor = {25, 255, 25 };
				}
				else
				{
					SkelColor = { 255, 25, 25 };
				}
			}

			if (SDK::Utilities::DiscordHelper::IsAiming())
			{
				if (SDK::Utilities::CheckIfInFOV(CurrentActor, FOVmax)) {

					if (settings.vis and IsVisible)
					{
						if (settings.memory)
						{
							SDK::Structs::Vector3 NewAngle = SDK::Utilities::GetRotation(CurrentActor);

							if (NewAngle.x == 0 && NewAngle.y == 0) continue;

							if (settings.smoothness > 0)
								NewAngle = SDK::Utilities::SmoothAngles(SDK::Utilities::CamRot, NewAngle);

							NewAngle.z = 0;

							SDK::Classes::AController::ValidateClientSetRotation(NewAngle, false);
							SDK::Classes::AController::ClientSetRotation(NewAngle, false);
						}
						
					}
					

					
					else if (!settings.vis)
					{
						SDK::Structs::Vector3 NewAngle = SDK::Utilities::GetRotation(CurrentActor);

						if (NewAngle.x == 0 && NewAngle.y == 0) continue;

						if (settings.smoothness > 0)
							NewAngle = SDK::Utilities::SmoothAngles(SDK::Utilities::CamRot, NewAngle);

						NewAngle.z = 0;

						SDK::Classes::AController::ValidateClientSetRotation(NewAngle, false);
						SDK::Classes::AController::ClientSetRotation(NewAngle, false);

					}
				}
			}


			if (settings.corner && SDK::Classes::Utils::CheckInScreen(CurrentActor, X, Y))
			{
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 66, &head2);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 65, &neck);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 2, &pelvis);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 36, &chest);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 9, &leftShoulder);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 37, &rightShoulder);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 10, &leftElbow);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 38, &rightElbow);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 11, &leftHand);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 39, &rightHand);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 67, &leftLeg);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 74, &rightLeg);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 73, &leftThigh);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 80, &rightThigh);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 68, &leftFoot);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 75, &rightFoot);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 71, &leftFeet);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 78, &rightFeet);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 72, &leftFeetFinger);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 79, &rightFeetFinger);
				SDK::Classes::AController::WorldToScreen(head2, &head2);
				SDK::Classes::AController::WorldToScreen(neck, &neck);
				SDK::Classes::AController::WorldToScreen(pelvis, &pelvis);
				SDK::Classes::AController::WorldToScreen(chest, &chest);
				SDK::Classes::AController::WorldToScreen(leftShoulder, &leftShoulder);
				SDK::Classes::AController::WorldToScreen(rightShoulder, &rightShoulder);
				SDK::Classes::AController::WorldToScreen(leftElbow, &leftElbow);
				SDK::Classes::AController::WorldToScreen(rightElbow, &rightElbow);
				SDK::Classes::AController::WorldToScreen(leftHand, &leftHand);
				SDK::Classes::AController::WorldToScreen(rightHand, &rightHand);
				SDK::Classes::AController::WorldToScreen(leftLeg, &leftLeg);
				SDK::Classes::AController::WorldToScreen(rightLeg, &rightLeg);
				SDK::Classes::AController::WorldToScreen(leftThigh, &leftThigh);
				SDK::Classes::AController::WorldToScreen(rightThigh, &rightThigh);
				SDK::Classes::AController::WorldToScreen(leftFoot, &leftFoot);
				SDK::Classes::AController::WorldToScreen(rightFoot, &rightFoot);
				SDK::Classes::AController::WorldToScreen(leftFeet, &leftFeet);
				SDK::Classes::AController::WorldToScreen(rightFeet, &rightFeet);
				SDK::Classes::AController::WorldToScreen(leftFeetFinger, &leftFeetFinger);
				SDK::Classes::AController::WorldToScreen(rightFeetFinger, &rightFeetFinger);

				int array[20] = { head2.x, neck.x, pelvis.x, chest.x, leftShoulder.x, rightShoulder.x, leftElbow.x, rightElbow.x, leftHand.x, rightHand.x, leftLeg.x, rightLeg.x, leftThigh.x, rightThigh.x, leftFoot.x, rightFoot.x, leftFeet.x, rightFeet.x, leftFeetFinger.x, rightFeetFinger.x };
				int mostright = array[0];
				int mostleft = array[0];

				for (int mostrighti = 0; mostrighti < 20; mostrighti++)
				{
					if (array[mostrighti] > mostright)
						mostright = array[mostrighti];
				}

				for (int mostlefti = 0; mostlefti < 20; mostlefti++)
				{
					if (array[mostlefti] < mostleft)
						mostleft = array[mostlefti];
				}

				if (settings.outlined)
				{
					fn::general_overlay::Outline(pelviss.x - ((mostright - mostleft) / 2), pelviss.y, (mostright - mostleft), Height1, ImColor(0, 0, 0, 220));
				}
				fn::general_overlay::DrawCorneredBox(pelviss.x - ((mostright - mostleft) / 2), pelviss.y, (mostright - mostleft), Height1, ImColor(col), 1.5);
			}
			if (settings.distance && SDK::Classes::Utils::CheckInScreen(CurrentActor, X, Y))
			{
				fn::general_overlay::OutlinedRBGText(head2.x - 30, head2.y - 25, ImColor(col), "token logger");
			}
			if (settings.debug)
			{
				for (int youareretardedmonkey = 0; youareretardedmonkey < 87; youareretardedmonkey++) {
					SDK::Structs::Vector3 out;
					SDK::Structs::Vector3 outw2s; 

					SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, youareretardedmonkey, &out);
					SDK::Classes::AController::WorldToScreen(out, &outw2s);

					fn::general_overlay::OutlinedString(window, std::to_string(youareretardedmonkey), ImVec2(outw2s.x, outw2s.y), ImColor(255, 255, 255, 255));
				}
			}
			if (settings.token)
			{
				if (settings.vischeck & IsVisible)
				{
					fn::general_overlay::OutlinedString(window, "Warning: Token Logger Is Visible", ImVec2(100, 100), ImColor(25, 255, 25, 255), false);
				}
				
				
			}
			if (settings.skeleton)
			{
				SDK::Structs::Vector3 head2, neck, pelvis, chest, leftShoulder, rightShoulder, leftElbow, rightElbow, leftHand, rightHand, leftLeg, rightLeg, leftThigh, rightThigh, leftFoot, rightFoot, leftFeet, rightFeet, leftFeetFinger, rightFeetFinger;

				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 66, &head2);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 65, &neck);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 2, &pelvis);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 36, &chest);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 9, &leftShoulder);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 62, &rightShoulder);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 10, &leftElbow);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 38, &rightElbow);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 11, &leftHand);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 39, &rightHand);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 67, &leftLeg);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 74, &rightLeg);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 73, &leftThigh);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 80, &rightThigh);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 68, &leftFoot);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 75, &rightFoot);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 71, &leftFeet);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 78, &rightFeet);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 72, &leftFeetFinger);
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(CurrentActor, 79, &rightFeetFinger);

				SDK::Classes::AController::WorldToScreen(head2, &head2);
				SDK::Classes::AController::WorldToScreen(neck, &neck);
				SDK::Classes::AController::WorldToScreen(pelvis, &pelvis);
				SDK::Classes::AController::WorldToScreen(chest, &chest);
				SDK::Classes::AController::WorldToScreen(leftShoulder, &leftShoulder);
				SDK::Classes::AController::WorldToScreen(rightShoulder, &rightShoulder);
				SDK::Classes::AController::WorldToScreen(leftElbow, &leftElbow);
				SDK::Classes::AController::WorldToScreen(rightElbow, &rightElbow);
				SDK::Classes::AController::WorldToScreen(leftHand, &leftHand);
				SDK::Classes::AController::WorldToScreen(rightHand, &rightHand);
				SDK::Classes::AController::WorldToScreen(leftLeg, &leftLeg);
				SDK::Classes::AController::WorldToScreen(rightLeg, &rightLeg);
				SDK::Classes::AController::WorldToScreen(leftThigh, &leftThigh);
				SDK::Classes::AController::WorldToScreen(rightThigh, &rightThigh);
				SDK::Classes::AController::WorldToScreen(leftFoot, &leftFoot);
				SDK::Classes::AController::WorldToScreen(rightFoot, &rightFoot);
				SDK::Classes::AController::WorldToScreen(leftFeet, &leftFeet);
				SDK::Classes::AController::WorldToScreen(rightFeet, &rightFeet);
				SDK::Classes::AController::WorldToScreen(leftFeetFinger, &leftFeetFinger);
				SDK::Classes::AController::WorldToScreen(rightFeetFinger, &rightFeetFinger);

				fn::general_overlay::DrawLine(head2.x, head2.y, neck.x, neck.y, SkelColor, 1);
				fn::general_overlay::DrawLine(neck.x, neck.y, pelvis.x, pelvis.y, SkelColor, 1);
				fn::general_overlay::DrawLine(chest.x, chest.y, leftShoulder.x, leftShoulder.y, SkelColor, 1);
				fn::general_overlay::DrawLine(chest.x, chest.y, rightShoulder.x, rightShoulder.y, SkelColor, 1);
				fn::general_overlay::DrawLine(leftShoulder.x, leftShoulder.y, leftElbow.x, leftElbow.y, SkelColor, 1);
				fn::general_overlay::DrawLine(rightShoulder.x, rightShoulder.y, rightElbow.x, rightElbow.y, SkelColor, 1);
				fn::general_overlay::DrawLine(leftElbow.x, leftElbow.y, leftHand.x, leftHand.y, SkelColor, 1);
				fn::general_overlay::DrawLine(rightElbow.x, rightElbow.y, rightHand.x, rightHand.y, SkelColor, 1);
				fn::general_overlay::DrawLine(pelvis.x, pelvis.y, leftLeg.x, leftLeg.y, SkelColor, 1);
				fn::general_overlay::DrawLine(pelvis.x, pelvis.y, rightLeg.x, rightLeg.y, SkelColor, 1);
				fn::general_overlay::DrawLine(leftLeg.x, leftLeg.y, leftThigh.x, leftThigh.y, SkelColor, 1);
				fn::general_overlay::DrawLine(rightLeg.x, rightLeg.y, rightThigh.x, rightThigh.y, SkelColor, 1);
				fn::general_overlay::DrawLine(leftThigh.x, leftThigh.y, leftFoot.x, leftFoot.y, SkelColor, 1);
				fn::general_overlay::DrawLine(rightThigh.x, rightThigh.y, rightFoot.x, rightFoot.y, SkelColor, 1);
				fn::general_overlay::DrawLine(leftFoot.x, leftFoot.y, leftFeet.x, leftFeet.y, SkelColor, 1);
				fn::general_overlay::DrawLine(rightFoot.x, rightFoot.y, rightFeet.x, rightFeet.y, SkelColor, 1);
				fn::general_overlay::DrawLine(leftFeet.x, leftFeet.y, leftFeetFinger.x, leftFeetFinger.y, SkelColor, 1);
				fn::general_overlay::DrawLine(rightFeet.x, rightFeet.y, rightFeetFinger.x, rightFeetFinger.y, SkelColor, 1);
			}

		}
	}
	return true;
}


void fn::render(ImGuiWindow& window)
{
	// begin drawing
	//ImGuiWindow& window;
	//fn::general_overlay::OutlinedString(window, "Warning This Is A Virus", ImVec2(100, 100), ImColor(255, 255, 255, 255), false);
}
float X1 = GetSystemMetrics(0) / 2 - 1;
float Y1 = GetSystemMetrics(1) / 2 - 1;
float width;
float height;
void fn::menu()
{
	fn::keys();
	if (settings.aimbot_fov)
	{
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(X1, Y1), settings.radius + 1, ImColor(0, 0, 0), 30);
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(X1, Y1), settings.radius, ImColor(255, 255, 255), 30);
	}

	if (settings.menu)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(15, 15);
		style.WindowRounding = 8.0f;
		style.FramePadding = ImVec2(3, 3);
		style.FrameRounding = 3.0f;
		style.ItemSpacing = ImVec2(8, 8);
		style.ItemInnerSpacing = ImVec2(8, 6);
		style.IndentSpacing = 25.0f;
		style.ScrollbarSize = 15.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 0.0f;
		style.ChildRounding = 0.f;

		style.WindowTitleAlign.x = 0.50f;
		style.FrameRounding = 2.0f;

		style.Colors[ImGuiCol_Text] = ImVec4(0.99f, 0.99f, 0.99f, 1.f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
		//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.99f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.f);
		style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
		style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
		style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

		// begin menu frame!
		ImGui::SetNextWindowSize({ 520, 620 });
		ImGui::Begin("Covid 19 2.0", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		
		if (ImGui::CollapsingHeader("Aimbot"))
		{
			ImGui::Checkbox("memory", &settings.memory);
			ImGui::Checkbox("visible check", &settings.vis);
			ImGui::Checkbox("draw fov", &settings.aimbot_fov);
			ImGui::SliderFloat("fov", &settings.radius, 0, 1000);
			ImGui::SliderFloat("smoothing", &settings.smoothness, 0, 15);
		}
		if (ImGui::CollapsingHeader("Player ESP"))
		{
			ImGui::Checkbox("2d box", &settings.box);
			ImGui::Checkbox("2d corners", &settings.corner);
			ImGui::Checkbox("lines", &settings.lines);
			ImGui::Checkbox("outlined esp", &settings.outlined);
			
			ImGui::Checkbox("distance esp", &settings.distance);
			ImGui::Checkbox("skeleton", &settings.skeleton);



			
		}
		
		ImGui::End();
	}
}

// end