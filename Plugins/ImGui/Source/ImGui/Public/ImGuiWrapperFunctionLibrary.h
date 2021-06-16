// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <imgui.h>
#include "ImGuiWrapperFunctionLibrary.generated.h"


class ConvertArrayFStringToArrayAnsi
{
public:
	ConvertArrayFStringToArrayAnsi(const TArray<FString>& Buffer)
	{
		size = Buffer.Num();
		if (size > 0)
		{
			ItemsIds = static_cast<const char**>(FMemory::Malloc(sizeof(char*) * size));
			TArray<FString> ItemNames;
			for (int32 Index = 0; Index < size; ++Index)
			{
				ItemsIds[Index] = static_cast<char*>(FMemory::Malloc(sizeof(char) * Buffer[Index].Len() + 1));
				FMemory::Memcpy(const_cast<char*>(ItemsIds[Index]), TCHAR_TO_UTF8(*Buffer[Index]), Buffer[Index].Len() + 1);
			}
		}
	}

	virtual ~ConvertArrayFStringToArrayAnsi()
	{
		for (int32 Index = 0; Index < size; ++Index)
		{
			FMemory::Free(const_cast<char*>(ItemsIds[Index]));
		}

		if (ItemsIds != nullptr)
		{
			FMemory::Free(ItemsIds);
			ItemsIds = nullptr;
		}
	}

	const char** GetData(){ return ItemsIds; } 

	int32 size = 0;
	const char** ItemsIds = nullptr;
};

DECLARE_DELEGATE(FUnWindingDelegate);

// Delegate
#define STORE_STACK_UNWINDING(Rewinder, Func) \
	static FUnWindingDelegate Delegate = FUnWindingDelegate::CreateStatic(Func);\
	Rewinder.Stack.Add(Delegate);

// Function
#define STORE_STACK_UNWINDING_FUNCTION(Rewinder, Func) \
	UFunction* Function = ThisClass::StaticClass()->FindFunctionByName(GET_MEMBER_NAME_CHECKED(ThisClass, Func));
	//Rewinder.Stack.Add(Function);


USTRUCT(BlueprintType)
struct FStackUnwinding
{
	GENERATED_USTRUCT_BODY()

	FStackUnwinding(){}
	
	void Excute()
	{
		for (int i = Stack.Num() - 1; i >= 0; --i)
		{
			Stack[i].Execute();
		}
	}

	void Clear()
	{
		Stack.Empty();
	}

	// (LIFO, instead of FIFO).	
	TArray<FUnWindingDelegate> Stack;
	
	//TArray<UFunction*> Stack;
};

UENUM()
namespace EImGuiButton
{
	enum Type
	{
		None		= 0 UMETA(DisplayName="None", ToolTip=""),
		Pressed		= 1 UMETA(DisplayName="Pressed", ToolTip=""),
	};
}

UENUM()
namespace EImGuiFlowControl
{
	enum Type
	{
		Success,
		Failure,
	};
}

inline ImVec2 ToImVec2(const FVector2D& vec)
{
	return ImVec2(vec.X, vec.Y);
}

inline FVector2D ToVector2D(const ImVec2& vec)
{
	return FVector2D(vec.x, vec.y);
}

inline ImVec4 ToImVec4(const FVector4& vec)
{
	return ImVec4(vec.X, vec.Y, vec.Z, vec.W);
}

inline ImVec4 ToImVec4(const FLinearColor& col)
{
	return ImVec4(col.R, col.G, col.B, col.A);
}


inline FVector4 ToVector4(const ImVec4& vec)
{
	return FVector4(vec.x, vec.y, vec.z, vec.w);
}

inline FLinearColor ToLinearColor(const ImVec4& vec)
{
	return FLinearColor(vec.x, vec.y, vec.z, vec.w);
}

/**
 *
 */
UCLASS()
class IMGUI_API UImGuiWrapperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static const int MAX_LENGTH = 256;
public:
	
	UFUNCTION(BlueprintCallable, Category = "ImGui|Context creation and access", meta = (DeprecatedFunction, DeprecationMessage = "Not implemented"))
	static void CreateContext() { ImGui::CreateContext(); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Context creation and access", meta = (DeprecatedFunction, DeprecationMessage = "Not implemented"))
	static void DestroyContext() { ImGui::DestroyContext(); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Context creation and access", meta = (DeprecatedFunction, DeprecationMessage = "Not implemented"))
	static void GetCurrentContext() { ImGui::GetCurrentContext(); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Context creation and access", meta = (DeprecatedFunction, DeprecationMessage = "Not implemented"))
	static void SetCurrentContext() { ImGui::SetCurrentContext(nullptr); }


	/** access the IO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags) */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Main", meta = (DeprecatedFunction, DeprecationMessage = "Not implemented"))
	static void GetIO() { ImGui::GetIO(); }

	/** access the Style structure (colors, sizes). Always use PushStyleCol(), PushStyleVar() to modify style mid-frame! */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Main")
	static void GetStyle() { ImGui::GetStyle(); }

	/** start a new Dear ImGui frame, you can submit any command from this point until Render()/EndFrame(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Main")
	static void NewFrame() { ImGui::NewFrame(); }

	/** ends the Dear ImGui frame. automatically called by Render(). If you don't need to render data (skipping rendering) you may call EndFrame() without Render()... but you'll have wasted CPU already! If you don't need to render, better to not create any windows and not call NewFrame() at all! */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Main")
	static void EndFrame() { ImGui::EndFrame(); }

	/** ends the Dear ImGui frame, finalize the draw data. You can then get call GetDrawData(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Main")
	static void Render() { ImGui::Render(); }

	/** valid after Render() and until the next call to NewFrame(). this is what you have to render. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Main", meta = (DeprecatedFunction, DeprecationMessage = "Not implemented"))
	static void GetDrawData() { ImGui::GetDrawData(); }


	/** create Demo window. demonstrate most ImGui features. call this to learn about the library! try to make it always available in your application! */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo, Debug, Information")
	static void ShowDemoWindow() { ImGui::ShowDemoWindow(); }

	/** create Metrics/Debugger window. display Dear ImGui internals: windows, draw commands, various internal state, etc. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo, Debug, Information")
	static void ShowMetricsWindow(UPARAM(ref) bool& open)
	{
		if(open) { ImGui::ShowMetricsWindow(&open); }		
	}

	/** create About window. display Dear ImGui version, credits and build/system information. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo, Debug, Information")
	static void ShowAboutWindow(bool open) { ImGui::ShowAboutWindow(&open); }

	/** add style editor block (not a window). you can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it uses the default style) */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo, Debug, Information")
	static void ShowStyleEditor() { ImGui::ShowStyleEditor(); }
	
	/** add style selector block (not a window), essentially a combo listing the default styles. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo, Debug, Information", meta = (ExpandEnumAsExecs="OutResult"))
	static void ShowStyleSelector(const FString& label, TEnumAsByte<EImGuiFlowControl::Type>& OutResult)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::ShowStyleSelector(TCHAR_TO_ANSI(*label)))
			OutResult = EImGuiFlowControl::Success;
	}

	/** add font selector block (not a window), essentially a combo listing the loaded fonts. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo, Debug, Information")
	static void ShowFontSelector(const FString& label) { ImGui::ShowFontSelector(TCHAR_TO_ANSI(*label)); }

	/** add basic help/info block (not a window): how to manipulate ImGui as a end-user (mouse/keyboard controls). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo, Debug, Information")
	static void ShowUserGuide() { ImGui::ShowUserGuide(); }

	/** get the compiled version string e.g. "1.80 WIP" (essentially the value for IMGUI_VERSION from the compiled version of imgui.cpp) */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Demo, Debug, Information", Meta = (ReturnDisplayName = "Version"))
	static FString GetVersion() { return ANSI_TO_TCHAR(ImGui::GetVersion()); }


	/* Styles */


	/** new, recommended style (default) */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Styles", meta = (DeprecatedFunction, DeprecationMessage = "Not implemented"))
	static void StyleColorsDark() { return ImGui::StyleColorsDark(); }

	/** best used with borders and a custom, thicker font */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Styles", meta = (DeprecatedFunction, DeprecationMessage = "Not implemented"))
	static void StyleColorsLight() { return ImGui::StyleColorsLight(); }

	/** classic imgui style */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Styles", meta = (DeprecatedFunction, DeprecationMessage = "Not implemented"))
	static void StyleColorsClassic() { return ImGui::StyleColorsClassic(); }

	
	/** Windows */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows", meta = (ExpandEnumAsExecs="OutResult", AdvancedDisplay = "1"))
	static void Begin(const FString& name, UPARAM(ref) bool& open, TEnumAsByte<EImGuiFlowControl::Type>& OutResult, int32 ImGuiWindowFlags = 0)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(open && ImGui::Begin(TCHAR_TO_ANSI(*name), &open, ImGuiWindowFlags))
		{
			OutResult = EImGuiFlowControl::Success;
		}
	}

	/** Windows */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows")
	static void End() { return ImGui::End(); }

	/** Child Windows */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Child Windows", meta = (ExpandEnumAsExecs="OutResult", AdvancedDisplay = "2"))
	static void BeginChild(const FString& str_id, FVector2D size, TEnumAsByte<EImGuiFlowControl::Type>& OutResult, bool border = false, int32 ImGuiWindowFlags = 0)
	{		
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::BeginChild(TCHAR_TO_ANSI(*str_id), ImVec2(size.X, size.Y), border, ImGuiWindowFlags))
		{
			OutResult = EImGuiFlowControl::Success;
		}
	}

	/** Child Windows */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Child Windows")
	static void EndChild() { return ImGui::EndChild(); }


	/* Windows Utilities */


	/** 'current window' = the window we are appending into while inside a Begin()/End() block. 'next window' = next window we will Begin() into. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static bool IsWindowAppearing() { return ImGui::IsWindowAppearing(); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static bool IsWindowCollapsed() { return ImGui::IsWindowCollapsed(); }

	/** is current window focused? or its root/child, depending on flags. see flags for options. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static bool IsWindowFocused(int32 ImGuiFocusedFlags) { return ImGui::IsWindowFocused(ImGuiFocusedFlags); }

	/** get draw list associated to the current window, to append your own drawing primitives */
	static ImDrawList* GetWindowDrawList() { return ImGui::GetWindowDrawList(); }

	/** get current window position in screen space (useful if you want to do your own drawing via the DrawList API) */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities", Meta = (ReturnDisplayName = "ScreenSpace"))
	static FVector2D GetWindowPos()
	{
		ImVec2 pos = ImGui::GetWindowPos();
		return FVector2D(pos.x, pos.y);
	}

	/** get current window size */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities", Meta = (ReturnDisplayName = "Size"))
	static FVector2D GetWindowSize()
	{
		ImVec2 size = ImGui::GetWindowPos();
		return FVector2D(size.x, size.y);
	}

	/** get current window width (shortcut for GetWindowSize().x) */
	UFUNCTION(BlueprintPure, Category = "ImGui|Windows Utilities", Meta = (ReturnDisplayName = "Width"))
	static float GetWindowWidth() { return ImGui::GetWindowWidth(); }

	/** get current window height (shortcut for GetWindowSize().y) */
	UFUNCTION(BlueprintPure, Category = "ImGui|Windows Utilities", Meta = (ReturnDisplayName = "Height"))
	static float GetWindowHeight() { return ImGui::GetWindowHeight(); }

	/** set next window position. call before Begin(). use pivot=(0.5f,0.5f) to center on given point, etc. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities", meta=(AdvancedDisplay = "2"))
	static void SetNextWindowPos(const FVector2D& pos, int32 cond = 0, FVector2D pivot = FVector2D(0, 0))
	{
		return ImGui::SetNextWindowPos(ToImVec2(pos), cond, ToImVec2(pivot));
	}

	/** set next window size. set axis to 0.0f to force an auto-fit on this axis. call before Begin() */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetNextWindowSize(const FVector2D& size, int32 cond = 0)
	{
		return ImGui::SetNextWindowPos(ToImVec2(size), cond);
	}

	/** set next window size limits. use -1,-1 on either X/Y axis to preserve the current size. Sizes will be rounded down. Use callback to apply non-trivial programmatic constraints. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities", meta = (DeprecatedFunction, DeprecationMessage = "Not implemented"))
	static void SetNextWindowSizeConstraints(const FVector2D& size_min, const FVector2D& size_max)
	{
		ImGui::SetNextWindowSizeConstraints(ToImVec2(size_min), ToImVec2(size_max), nullptr, nullptr);
	}

	/** set next window content size (~ scrollable client area, which enforce the range of scrollbars). Not including window decorations (title bar, menu bar, etc.) nor WindowPadding. set an axis to 0.0f to leave it automatic. call before Begin() */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetNextWindowContentSize(const FVector2D& size) { ImGui::SetNextWindowContentSize(ToImVec2(size)); }

	/** set next window collapsed state. call before Begin() */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetNextWindowCollapsed(bool collapsed, int32 cond = 0)
	{
		ImGui::SetNextWindowCollapsed(collapsed, cond);
	}

	/** set next window to be focused / top-most. call before Begin() */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetNextWindowFocus() { ImGui::SetNextWindowFocus(); }

	/** set next window background color alpha. helper to easily override the Alpha component of ImGuiCol_WindowBg/ChildBg/PopupBg. you may also use ImGuiWindowFlags_NoBackground. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetNextWindowBgAlpha(float alpha) { ImGui::SetNextWindowBgAlpha(alpha); }

	/** set font scale. Adjust IO.FontGlobalScale if you want to scale all windows. This is an old API! For correct scaling, prefer to reload font + rebuild ImFontAtlas + call style.ScaleAllSizes(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetWindowFontScale(float scale) { ImGui::SetWindowFontScale(scale); }

	/** (not recommended) set current window position - call within Begin()/End(). prefer using SetNextWindowPos(), as this may incur tearing and side-effects. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetWindowPos(const FVector2D& pos, int32 cond = 0) { ImGui::SetWindowPos(ImVec2(pos.X, pos.Y), cond); }

	/** set named window position. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetWindowPosA(const FString& name, const FVector2D& pos, int32 cond = 0)
	{
		ImGui::SetWindowPos(TCHAR_TO_ANSI(*name), ImVec2(pos.X, pos.Y), cond);
	}

	/** (not recommended) set current window size - call within Begin()/End(). set to ImVec2(0, 0) to force an auto-fit. prefer using SetNextWindowSize(), as this may incur tearing and minor side-effects. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetWindowSize(const FVector2D& size, int32 cond = 0)
	{
		ImGui::SetWindowSize(ImVec2(size.X, size.Y), cond);
	}

	/** set named window size. set axis to 0.0f to force an auto-fit on this axis. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetWindowSizeA(const FString& name, const FVector2D& size, int32 cond = 0)
	{
		ImGui::SetWindowSize(TCHAR_TO_ANSI(*name), ImVec2(size.X, size.Y), cond);
	}

	/** (not recommended) set current window collapsed state. prefer using SetNextWindowCollapsed(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetWindowCollapsed(bool collapsed, int32 cond = 0) { ImGui::SetWindowCollapsed(collapsed, cond); }

	/** set named window collapsed state */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetWindowCollapsedA(const FString& name, bool collapsed, int32 cond = 0)
	{
		ImGui::SetWindowCollapsed(TCHAR_TO_ANSI(*name), collapsed, cond);
	}

	/** (not recommended) set current window to be focused / top-most. prefer using SetNextWindowFocus(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetWindowFocus() { return ImGui::SetWindowFocus(); }

	/** set named window to be focused / top-most. use NULL to remove focus. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Utilities")
	static void SetWindowFocusA(const FString& name) { ImGui::SetWindowFocus(TCHAR_TO_ANSI(*name)); }


	/* Content region */


	/** == GetContentRegionMax() - GetCursorPos() */
	UFUNCTION(BlueprintPure, Category = "ImGui|Content region")
	static FVector2D GetContentRegionAvail() { return ToVector2D(ImGui::GetContentRegionAvail()); }

	/** current content boundaries (typically window boundaries including scrolling, or current column boundaries), in windows coordinates */
	UFUNCTION(BlueprintPure, Category = "ImGui|Content region", Meta = (ReturnDisplayName = "Coordinates"))
	static FVector2D GetContentRegionMax() { return ToVector2D(ImGui::GetContentRegionMax()); }

	/** content boundaries min (roughly (0,0)-Scroll), in window coordinates */
	UFUNCTION(BlueprintPure, Category = "ImGui|Content region", Meta = (ReturnDisplayName = "Coordinates"))
	static FVector2D GetWindowContentRegionMin() { return ToVector2D(ImGui::GetWindowContentRegionMin()); }

	/** content boundaries max (roughly (0,0)+Size-Scroll) where Size can be override with SetNextWindowContentSize(), in window coordinates */
	UFUNCTION(BlueprintPure, Category = "ImGui|Content region", Meta = (ReturnDisplayName = "Coordinates"))
	static FVector2D GetWindowContentRegionMax() { return ToVector2D(ImGui::GetWindowContentRegionMax()); }

	/**  */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Content region")
	static float GetWindowContentRegionWidth() { return ImGui::GetWindowContentRegionWidth(); }


	/* Windows Scrolling */


	/** get scrolling amount [0 .. GetScrollMaxX()] */
	UFUNCTION(BlueprintPure, Category = "ImGui|Windows Scrolling")
	static float GetScrollX() { return ImGui::GetScrollX(); }

	/** get scrolling amount [0 .. GetScrollMaxY()] */
	UFUNCTION(BlueprintPure, Category = "ImGui|Windows Scrolling")
	static float GetScrollY() { return ImGui::GetScrollY(); }

	/** set scrolling amount [0 .. GetScrollMaxX()] */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Scrolling")
	static void SetScrollX(float scroll_x) { ImGui::SetScrollX(scroll_x); }

	/** set scrolling amount [0 .. GetScrollMaxY()] */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Scrolling")
	static void SetScrollY(float scroll_y) { ImGui::SetScrollY(scroll_y); }

	/** get maximum scrolling amount ~~ ContentSize.x - WindowSize.x - DecorationsSize.x */
	UFUNCTION(BlueprintPure, Category = "ImGui|Windows Scrolling")
	static float GetScrollMaxX() { return ImGui::GetScrollMaxX(); }

	/** get maximum scrolling amount ~~ ContentSize.y - WindowSize.y - DecorationsSize.y */
	UFUNCTION(BlueprintPure, Category = "ImGui|Windows Scrolling")
	static float GetScrollMaxY() { return ImGui::GetScrollMaxY(); }

	/** adjust scrolling amount to make current cursor position visible. center_x_ratio=0.0: left, 0.5: center, 1.0: right. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Scrolling")
	static void SetScrollHereX(float center_x_ratio = 0.5f) { ImGui::SetScrollHereX(center_x_ratio); }

	/** adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Scrolling")
	static void SetScrollHereY(float center_y_ratio = 0.5f) { ImGui::SetScrollHereY(center_y_ratio); }

	/** adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Scrolling")
	static void SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f)
	{
		ImGui::SetScrollFromPosX(local_x, center_x_ratio);
	}

	/** adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Windows Scrolling")
	static void SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f)
	{
		ImGui::SetScrollFromPosY(local_y, center_y_ratio);
	}


	/* Parameters stacks (shared) */

	/** use NULL as a shortcut to push default font */
	static void PushFont(ImFont* font) { ImGui::PushFont(font); };
	static void PopFont();

	// modify a style color. always use this if you modify the style after NewFrame().
	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (shared)")
	static void PushStyleColor(int32 ImGuiCol, int32 col) { ImGui::PushStyleColor(ImGuiCol, col); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (shared)")
	static void PushStyleColorA(int32 ImGuiCol, const FVector4& col) { ImGui::PushStyleColor(ImGuiCol, ToImVec4(col)); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (shared)")
	static void PopStyleColor(int count = 1) { ImGui::PopStyleColor(count); }

	// modify a style float variable. always use this if you modify the style after NewFrame().
	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (shared)")
	static void PushStyleVar(int32 ImGuiStyleVar, float val) { ImGui::PushStyleVar(ImGuiStyleVar, val); }

	// modify a style ImVec2 variable. always use this if you modify the style after NewFrame().
	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (shared)")
	static void PushStyleVarA(int32 ImGuiStyleVar, const FVector2D& val)
	{
		ImGui::PushStyleVar(ImGuiStyleVar, ToImVec2(val));
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (shared)")
	static void PopStyleVar(int count = 1) { ImGui::PopStyleVar(count); }

	// allow focusing using TAB/Shift-TAB, enabled by default but you can disable it for certain widgets
	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (shared)")
	static void PushAllowKeyboardFocus(bool allow_keyboard_focus)
	{
		ImGui::PushAllowKeyboardFocus(allow_keyboard_focus);
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (shared)")
	static void PopAllowKeyboardFocus() { ImGui::PopAllowKeyboardFocus(); }

	// in 'repeat' mode, Button*() functions return repeated true in a typematic manner (using io.KeyRepeatDelay/io.KeyRepeatRate setting). Note that you can call IsItemActive() after any Button() to tell if the button is held in the current frame.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (shared)")
	static void PushButtonRepeat(bool repeat) { ImGui::PushButtonRepeat(repeat); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (shared)")
	static void PopButtonRepeat() { ImGui::PopButtonRepeat(); }


	/* Parameters stacks (current window) */


	// push width of items for common large "item+label" widgets. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN always align width to the right side).
	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (current window)")
	static void PushItemWidth(float item_width) { ImGui::PushItemWidth(item_width); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (current window)")
	static void PopItemWidth() { ImGui::PopItemWidth(); }

	// set width of the _next_ common large "item+label" widget. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN always align width to the right side)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (current window)")
	static void SetNextItemWidth(float item_width) { ImGui::SetNextItemWidth(item_width); }

	// width of item given pushed settings and current cursor position. NOT necessarily the width of last item unlike most 'Item' functions.
	UFUNCTION(BlueprintPure, Category = "ImGui|Parameters stacks (current window)")
	static float CalcItemWidth() { return ImGui::CalcItemWidth(); }

	// push word-wrapping position for Text*() commands. < 0.0f: no wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrap_pos_x' position in window local space
	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (current window)")
	static void PushTextWrapPos(float wrap_local_pos_x = 0.0f) { ImGui::PushTextWrapPos(wrap_local_pos_x); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Parameters stacks (current window)")
	static void PopTextWrapPos() { ImGui::PopTextWrapPos(); }

	/*Style read access*/

	// get current font
	//UFUNCTION(BlueprintCallable, Category = "ImGui|Style read access")
	static ImFont* GetFont() { return ImGui::GetFont(); }

	// get current font size (= height in pixels) of current font with current scale applied
	UFUNCTION(BlueprintCallable, Category = "ImGui|Style read access")
	static float GetFontSize() { return ImGui::GetFontSize(); }

	// get UV coordinate for a while pixel, useful to draw custom shapes via the ImDrawList API
	UFUNCTION(BlueprintCallable, Category = "ImGui|Style read access", Meta = (ReturnDisplayName = "UVPixel"))
	static FVector2D GetFontTexUvWhitePixel() { return ToVector2D(ImGui::GetFontTexUvWhitePixel()); }

	// retrieve given style color with style alpha applied and optional extra alpha multiplier, packed as a 32-bit value suitable for ImDrawList
	UFUNCTION(BlueprintPure, Category = "ImGui|Style read access")
	static int32 GetColorU32(int32 ImGuiCol, float alpha_mul = 1.0f) { return ImGui::GetColorU32(ImGuiCol, alpha_mul); }

	// retrieve given color with style alpha applied, packed as a 32-bit value suitable for ImDrawList
	UFUNCTION(BlueprintPure, Category = "ImGui|Style read access")
	static int32 GetColorU32A(const FVector4& col) { return ImGui::GetColorU32(ToImVec4(col)); }

	// retrieve given color with style alpha applied, packed as a 32-bit value suitable for ImDrawList
	UFUNCTION(BlueprintPure, Category = "ImGui|Style read access")
	static int32 GetColorU32B(int32 col) { return ImGui::GetColorU32((ImU32)col); }

	// retrieve style color as stored in ImGuiStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in.
	UFUNCTION(BlueprintPure, Category = "ImGui|Style read access", Meta = (ReturnDisplayName = "Color"))
	static FLinearColor GetStyleColorVec4(int32 ImGuiCol) { return ToLinearColor(ImGui::GetStyleColorVec4(ImGuiCol)); }


	/* Cursor / Layout */


	// separator, generally horizontal. inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout")
	static void Separator() { ImGui::Separator(); }

	// call between widgets or groups to layout them horizontally. X position given in window coordinates.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout", meta = (AdvancedDisplay = "offset_from_start_x,spacing"))
	static void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f)
	{
		ImGui::SameLine(offset_from_start_x, spacing);
	}

	// undo a SameLine() or force a new line when in an horizontal-layout context.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout")
	static void NewLine() { ImGui::NewLine(); }

	// add vertical spacing.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout")
	static void Spacing() { ImGui::Spacing(); }

	// add a dummy item of given size. unlike InvisibleButton(), Dummy() won't take the mouse click or be navigable into.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout")
	static void Dummy(const FVector2D& size) { ImGui::Dummy(ToImVec2(size)); }

	// move content position toward the right, by indent_w, or style.IndentSpacing if indent_w <= 0
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout", meta = (AdvancedDisplay = "0"))
	static void Indent(float indent_w = 0.0f) { ImGui::Indent(indent_w); }

	// move content position back to the left, by indent_w, or style.IndentSpacing if indent_w <= 0
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout", meta = (AdvancedDisplay = "0"))
	static void Unindent(float indent_w = 0.0f) { ImGui::Unindent(indent_w); }

	// lock horizontal starting position
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout")
	static void BeginGroup()
	{
		ImGui::BeginGroup();
	}

	// unlock horizontal starting position + capture the whole group bounding box into one "item" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout")
	static void EndGroup() { ImGui::EndGroup(); }

	// cursor position in window coordinates (relative to window position)
	UFUNCTION(BlueprintPure, Category = "ImGui|Cursor / Layout")
	static FVector2D GetCursorPos() { return ToVector2D(ImGui::GetCursorPos()); }

	//   (some functions are using window-relative coordinates, such as: GetCursorPos, GetCursorStartPos, GetContentRegionMax, GetWindowContentRegion* etc.
	UFUNCTION(BlueprintPure, Category = "ImGui|Cursor / Layout")
	static float GetCursorPosX() { return ImGui::GetCursorPosX(); }

	//    other functions such as GetCursorScreenPos or everything in ImDrawList::
	UFUNCTION(BlueprintPure, Category = "ImGui|Cursor / Layout")
	static float GetCursorPosY() { return ImGui::GetCursorPosY(); }

	//    are using the main, absolute coordinate system.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout")
	static void SetCursorPos(const FVector2D& local_pos) { ImGui::SetCursorPos(ToImVec2(local_pos)); }

	//    GetWindowPos() + GetCursorPos() == GetCursorScreenPos() etc.)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout")
	static void SetCursorPosX(float local_x) { ImGui::SetCursorPosX(local_x); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout")
	static void SetCursorPosY(float local_y) { ImGui::SetCursorPosX(local_y); }

	// initial cursor position in window coordinates
	UFUNCTION(BlueprintPure, Category = "ImGui|Cursor / Layout", Meta = (ReturnDisplayName = "CursorPos"))
	static FVector2D GetCursorStartPos() { return ToVector2D(ImGui::GetCursorStartPos()); }

	// cursor position in absolute coordinates (useful to work with ImDrawList API). generally top-left == GetMainViewport()->Pos == (0,0) in single viewport mode, and bottom-right == GetMainViewport()->Pos+Size == io.DisplaySize in single-viewport mode.
	UFUNCTION(BlueprintPure, Category = "ImGui|Cursor / Layout", Meta = (ReturnDisplayName = "CursorPos"))
	static FVector2D GetCursorScreenPos() { return ToVector2D(ImGui::GetCursorScreenPos()); }

	// cursor position in absolute coordinates
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout")
	static void SetCursorScreenPos(const FVector2D& pos) { ImGui::SetCursorScreenPos(ToImVec2(pos)); }

	// vertically align upcoming text baseline to FramePadding.y so that it will align properly to regularly framed items (call if you have text on a line before a framed item)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Cursor / Layout")
	static void AlignTextToFramePadding() { ImGui::AlignTextToFramePadding(); }

	// ~ FontSize
	UFUNCTION(BlueprintPure, Category = "ImGui|Cursor / Layout")
	static float GetTextLineHeight() { return ImGui::GetTextLineHeight(); }

	// ~ FontSize + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of text)
	UFUNCTION(BlueprintPure, Category = "ImGui|Cursor / Layout")
	static float GetTextLineHeightWithSpacing() { return ImGui::GetTextLineHeightWithSpacing(); }

	// ~ FontSize + style.FramePadding.y * 2
	UFUNCTION(BlueprintPure, Category = "ImGui|Cursor / Layout")
	static float GetFrameHeight() { return ImGui::GetFrameHeight(); }

	// ~ FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of framed widgets)
	UFUNCTION(BlueprintPure, Category = "ImGui|Cursor / Layout")
	static float GetFrameHeightWithSpacing() { return ImGui::GetFrameHeightWithSpacing(); }


	/* ID stack/scopes */


	// push string into the ID stack (will hash string).
	UFUNCTION(BlueprintCallable, Category = "ImGui|ID stack/scopes")
	static void PushID(const FString& str_id) { ImGui::PushID(TCHAR_TO_ANSI(*str_id)); }

	// push string into the ID stack (will hash string).
	UFUNCTION(BlueprintCallable, Category = "ImGui|ID stack/scopes")
	static void PushID_A(const FString& str_id_begin, const FString& str_id_end)
	{
		ImGui::PushID(TCHAR_TO_ANSI(*str_id_begin), TCHAR_TO_ANSI(*str_id_end));
	}

	// push pointer into the ID stack (will hash pointer).
	//UFUNCTION(BlueprintCallable, Category = "ImGui|ID stack/scopes")
	static void PushID_B(const void* ptr_id) { ImGui::PushID(ptr_id); }

	// push integer into the ID stack (will hash integer).
	UFUNCTION(BlueprintCallable, Category = "ImGui|ID stack/scopes")
	static void PushID_C(int32 int_id) { ImGui::PushID(int_id); }

	// pop from the ID stack.
	UFUNCTION(BlueprintCallable, Category = "ImGui|ID stack/scopes")
	static void PopID() { ImGui::PopID(); }

	// calculate unique ID (hash of whole ID stack + given parameter). e.g. if you want to query into ImGuiStorage yourself
	UFUNCTION(BlueprintPure, Category = "ImGui|ID stack/scopes")
	static int32 GetID(const FString& str_id) { return ImGui::GetID(TCHAR_TO_ANSI(*str_id)); }

	UFUNCTION(BlueprintPure, Category = "ImGui|ID stack/scopes")
	static int32 GetID_A(const FString& str_id_begin, const FString& str_id_end)
	{
		return ImGui::GetID(TCHAR_TO_ANSI(*str_id_begin), TCHAR_TO_ANSI(*str_id_end));
	}

	//UFUNCTION(BlueprintPure, Category = "ImGui|ID stack/scopes")
	static int32 GetID_B(const void* ptr_id) { return ImGui::GetID(ptr_id); }


	/* Widgets: Text */

	// raw text without formatting. Roughly equivalent to Text("%s", text) but: A) doesn't require null terminated string if 'text_end' is specified, B) it's faster, no memory copy is done, no buffer size limits, recommended for long chunks of text.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Text")
	static void TextUnformatted(const FString& text, const FString& text_end)
	{
		ImGui::TextUnformatted(TCHAR_TO_ANSI(*text), TCHAR_TO_ANSI(*text_end));
	}

	// formatted text
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Text")
	static void Text(const FString& text) { ImGui::Text(TCHAR_TO_ANSI(*text), nullptr); }

	// shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Text")
	static void TextColored(const FVector4& col, const FString& text)
	{
		ImGui::TextColored(ToImVec4(col), TCHAR_TO_ANSI(*text), nullptr);
	}

	// shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Text")
	static void TextDisabled(const FString& text) { ImGui::TextDisabled(TCHAR_TO_ANSI(*text), nullptr); }

	// shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Text")
	static void TextWrapped(const FString& text) { ImGui::TextWrapped(TCHAR_TO_ANSI(*text), nullptr); }

	// display text+label aligned the same way as value+label widgets
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Text")
	static void LabelText(const FString& label, const FString& text)
	{
		ImGui::LabelText(TCHAR_TO_ANSI(*label), TCHAR_TO_ANSI(*text), nullptr);
	}

	// shortcut for Bullet()+Text()
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Text")
	static void BulletText(const FString& text) { ImGui::BulletText(TCHAR_TO_ANSI(*text), nullptr); }


	/* Widgets: Main */


	// button
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main")
	static bool Button(const FString& label, FVector2D size)
	{
		return ImGui::Button(TCHAR_TO_ANSI(*label), ToImVec2(size));
	}

	// button with FramePadding=(0,0) to easily embed within text
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main")
	static bool SmallButton(const FString& label) { return ImGui::SmallButton(TCHAR_TO_ANSI(*label)); }

	// flexible button behavior without the visuals, frequently useful to build custom behaviors using the public api (along with IsItemActive, IsItemHovered, etc.)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main")
	static bool InvisibleButton(const FString& str_id, const FVector2D& size, int32 ImGuiButtonFlags = 0)
	{
		return ImGui::InvisibleButton(TCHAR_TO_ANSI(*str_id), ToImVec2(size), ImGuiButtonFlags);
	}

	// square button with an arrow shape
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main")
	static bool ArrowButton(const FString& str_id, int32 ImGuiDir)
	{
		return ImGui::ArrowButton(TCHAR_TO_ANSI(*str_id), ImGuiDir);
	}

	//UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main")
	//static void Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1,1), const ImVec4& tint_col = ImVec4(1,1,1,1), const ImVec4& border_col = ImVec4(0,0,0,0))
	//{ return ImGui::Image();}

	// <0 frame_padding uses default frame padding settings. 0 for no padding
	//UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main")
	//static bool ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0),  const ImVec2& uv1 = ImVec2(1,1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0,0,0,0), const ImVec4& tint_col = ImVec4(1,1,1,1))
	//{ return ImGui::ImageButton();}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main", meta = (ExpandEnumAsExecs="OutResult"))
	static void Checkbox(const FString& label, UPARAM(ref) bool& v, TEnumAsByte<EImGuiButton::Type>& OutResult)
	{
		if(ImGui::Checkbox(TCHAR_TO_ANSI(*label), &v))
			OutResult = EImGuiButton::Pressed;
		else
			OutResult = EImGuiButton::None;
	}
	
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main")
	static bool CheckboxFlags(const FString& label, UPARAM(ref) int32& flags, int flags_value)
	{
		return ImGui::CheckboxFlags(TCHAR_TO_ANSI(*label), &flags, flags_value);
	}

	// use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1; }
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main", meta = (ExpandEnumAsExecs="OutResult"))
	static void RadioButton(const FString& label, bool active, TEnumAsByte<EImGuiButton::Type>& OutResult)
	{
		if(ImGui::RadioButton(TCHAR_TO_ANSI(*label), active))
			OutResult = EImGuiButton::Pressed;
		else
			OutResult = EImGuiButton::None;
	}

	// shortcut to handle the above pattern when value is an integer
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main", meta = (ExpandEnumAsExecs="OutResult"))
	static void RadioButtonA(const FString& label, UPARAM(ref) int32& v, int v_button, TEnumAsByte<EImGuiFlowControl::Type>& OutResult)
	{
		if(ImGui::RadioButton(TCHAR_TO_ANSI(*label), &v, v_button))
			OutResult = EImGuiFlowControl::Success;
		else
			OutResult = EImGuiFlowControl::Failure;
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main") //MIN_flt
	static void ProgressBar(float fraction, FVector2D size_arg = FVector2D(-1.175494351e-38F, 0.0f),
	                        FString overlay = TEXT(""))
	{
		ImGui::ProgressBar(fraction, ToImVec2(size_arg), TCHAR_TO_ANSI(*overlay));
	}

	// draw a small circle + keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Main")
	static void Bullet() { ImGui::Bullet(); }


	/* Widgets: Combo Box */


	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Combo Box")
	static bool BeginCombo(const FString& label, const FString& preview_value, int32 ImGuiComboFlags = 0)
	{
		return ImGui::BeginCombo(TCHAR_TO_ANSI(*label), TCHAR_TO_ANSI(*preview_value), ImGuiComboFlags);
	}

	// only call EndCombo() if BeginCombo() returns true!
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Combo Box")
	static void EndCombo() { ImGui::EndCombo(); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Combo Box")
	static bool Combo(const FString& label, UPARAM(ref) int32& current_item, const TArray<FString>& items,
	                  int items_count, int popup_max_height_in_items = -1);

	// Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Combo Box")
	static bool ComboA(const FString& label, UPARAM(ref) int32& current_item, const FString& items_separated_by_zeros,
	                   int popup_max_height_in_items = -1)
	{
		return ImGui::Combo(TCHAR_TO_ANSI(*label), &current_item, TCHAR_TO_ANSI(*items_separated_by_zeros),
		                    popup_max_height_in_items);
	}

	// UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Combo Box")
	// static bool ComboB(const FString& label, UPARAM(ref) int32& current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1)
	// { return ImGui::Combo(TCHAR_TO_ANSI(*label), current_item);}


	/* Widgets: Drag Sliders */


	// If v_min >= v_max we have no bound
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Drag Sliders", meta = (ExpandEnumAsExecs="OutResult", AdvancedDisplay = "3"))
	static void DragFloat(const FString& label, UPARAM(ref) float& Value, float v_speed, TEnumAsByte<EImGuiFlowControl::Type>& OutResult,
					float p_min = -1.0f, float p_max = 1.0f, FString format = TEXT("%.3f"), int32 ImGuiSliderFlags = 0)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::DragScalarN(TCHAR_TO_ANSI(*label), ImGuiDataType_Float, &Value, 1, v_speed,
		                          &p_min, &p_max, TCHAR_TO_ANSI(*format), ImGuiSliderFlags))
		{
			OutResult = EImGuiFlowControl::Success;
		}
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Drag Sliders", meta = (ExpandEnumAsExecs="OutResult", AdvancedDisplay = "3"))
	static void DragArrayFloat(const FString& label, UPARAM(ref) TArray<float>& data, float v_speed, TEnumAsByte<EImGuiFlowControl::Type>& OutResult
				, float p_min = -1.0f, float p_max = 1.0f, FString format = TEXT("%.3f"), int32 ImGuiSliderFlags = 0)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::DragScalarN(TCHAR_TO_ANSI(*label), ImGuiDataType_Float, data.GetData(), data.Num(), v_speed,
								&p_min, &p_max, TCHAR_TO_ANSI(*format), ImGuiSliderFlags))
		{
			OutResult = EImGuiFlowControl::Success;
		}
	}
	
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Drag Sliders", meta = (ExpandEnumAsExecs="OutResult"))
	static void DragFloatRange2(const FString& label, UPARAM(ref) float& v_current_min,
	                            UPARAM(ref) float& v_current_max, TEnumAsByte<EImGuiFlowControl::Type>& OutResult,
								float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, 
								FString format = TEXT("%.3f"), FString format_max = TEXT(""), int32 ImGuiSliderFlags = 0)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::DragFloatRange2(TCHAR_TO_ANSI(*label), &v_current_min, &v_current_max, v_speed, v_min, v_max,
		                              TCHAR_TO_ANSI(*format), format_max.Len() > 0 ? TCHAR_TO_ANSI(*format_max) : nullptr, ImGuiSliderFlags))
			OutResult = EImGuiFlowControl::Success;
	}

	// If v_min >= v_max we have no bound
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Drag Sliders")
	static bool DragInt(const FString& label, TArray<int32> data, float v_speed, int32 p_min = -10, int32 p_max = 10,
	                    FString format = TEXT("%d"), int32 ImGuiSliderFlags = 0)
	{
		return ImGui::DragScalarN(TCHAR_TO_ANSI(*label), ImGuiDataType_S32, data.GetData(), data.Num(), v_speed, &p_min,
		                          &p_max, TCHAR_TO_ANSI(*format), ImGuiSliderFlags);
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Drag Sliders")
	static bool DragIntRange2(const FString& label, UPARAM(ref) int32& v_current_min, UPARAM(ref) int32& v_current_max,
	                          float v_speed = 1.0f, int v_min = 0, int v_max = 0, FString format = TEXT("%d"),
	                          FString format_max = TEXT(""), int32 ImGuiSliderFlags = 0)
	{
		return ImGui::DragIntRange2(TCHAR_TO_ANSI(*label), &v_current_min, &v_current_max, v_speed, v_min, v_max,
		                            TCHAR_TO_ANSI(*format), TCHAR_TO_ANSI(*format_max), ImGuiSliderFlags);
	}


	/* Widgets: Regular Sliders */


	// adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Regular Sliders", meta = (ExpandEnumAsExecs="OutResult"))
	static void SliderFloat(const FString& label, UPARAM(ref) float& v, float v_min, float v_max, TEnumAsByte<EImGuiFlowControl::Type>& OutResult,
	                        FString format = TEXT("%.3f"), int32 ImGuiSliderFlags = 0)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::SliderScalarN(TCHAR_TO_ANSI(*label), ImGuiDataType_Float, &v, 1, &v_min, &v_max,
									TCHAR_TO_ANSI(*format), ImGuiSliderFlags))
		{
			OutResult = EImGuiFlowControl::Success;
		}
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Regular Sliders", meta = (ExpandEnumAsExecs="OutResult"))
	static void SliderArrayFloat(const FString& label, UPARAM(ref) TArray<float>& v, float v_min, float v_max, TEnumAsByte<EImGuiFlowControl::Type>& OutResult,
						FString format = TEXT("%.3f"), int32 ImGuiSliderFlags = 0)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::SliderScalarN(TCHAR_TO_ANSI(*label), ImGuiDataType_Float, v.GetData(), v.Num(), &v_min, &v_max,
									TCHAR_TO_ANSI(*format), ImGuiSliderFlags))
		{
			OutResult = EImGuiFlowControl::Success;
		}
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Regular Sliders")
	static bool SliderAngle(const FString& label, UPARAM(ref) float& v_rad, float v_degrees_min = -360.0f,
	                        float v_degrees_max = +360.0f, FString format = TEXT("%.0f deg"),
	                        int32 ImGuiSliderFlags = 0)
	{
		return ImGui::SliderAngle(TCHAR_TO_ANSI(*label), &v_rad, v_degrees_min, v_degrees_max, TCHAR_TO_ANSI(*format),
		                          ImGuiSliderFlags);
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Regular Sliders", meta = (ExpandEnumAsExecs="OutResult"))
	static void SliderInt(const FString& label, UPARAM(ref) int32& v, int32 v_min, int32 v_max, TEnumAsByte<EImGuiFlowControl::Type>& OutResult, FString format = TEXT("%d"),
						int32 ImGuiSliderFlags = 0)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::SliderScalarN(TCHAR_TO_ANSI(*label), ImGuiDataType_S32, &v, 1, &v_min, &v_max,
									TCHAR_TO_ANSI(*format), ImGuiSliderFlags))
		{
			OutResult = EImGuiFlowControl::Success;
		}
	}
	
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Regular Sliders", meta = (ExpandEnumAsExecs="OutResult"))
	static void SliderArrayInt(const FString& label, UPARAM(ref) TArray<int32>& v, int32 v_min, int32 v_max, TEnumAsByte<EImGuiFlowControl::Type>& OutResult
								, FString format = TEXT("%d"), int32 ImGuiSliderFlags = 0)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::SliderScalarN(TCHAR_TO_ANSI(*label), ImGuiDataType_S32, v.GetData(), v.Num(), &v_min, &v_max,
		                            TCHAR_TO_ANSI(*format), ImGuiSliderFlags))
		{
			OutResult = EImGuiFlowControl::Success;
		}
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Regular Sliders", meta = (ExpandEnumAsExecs="OutResult"))
	static void VSliderFloat(const FString& label, const FVector2D& size, UPARAM(ref) float& v, float v_min, float v_max, TEnumAsByte<EImGuiFlowControl::Type>& OutResult
							, FString format = TEXT("%.3f"), int32 ImGuiSliderFlags = 0)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::VSliderFloat(TCHAR_TO_ANSI(*label), ToImVec2(size), &v, v_min, v_max, TCHAR_TO_ANSI(*format),
		                           ImGuiSliderFlags))
		{
			OutResult = EImGuiFlowControl::Success;
		}
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Regular Sliders", meta = (ExpandEnumAsExecs="OutResult"))
	static void VSliderInt(const FString& label, const FVector2D& size, UPARAM(ref) int32& v, int v_min, int v_max, TEnumAsByte<EImGuiFlowControl::Type>& OutResult,
	                       FString format = TEXT("%d"), int32 ImGuiSliderFlags = 0)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::VSliderInt(TCHAR_TO_ANSI(*label), ToImVec2(size), &v, v_min, v_max, TCHAR_TO_ANSI(*format),
								ImGuiSliderFlags))
		{
			OutResult = EImGuiFlowControl::Success;
		}
	}


	/* Widgets: Input with Keyboard */

	//DECLARE_DYNAMIC_DELEGATE_OneParam(FImGuiInputTextCallback, FImGuiInputTextCallbackData, callbackData);
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Input with Keyboard")
	static bool InputText(const FString& label, UPARAM(ref) FString& OutBuf, int32 ImGuiInputTextFlags = 0 /*, const FImGuiInputTextCallback& InputTextCallback, void* user_data = NULL*/)
	{
		auto replacementLambda = [](ImGuiInputTextCallbackData* data)
		{
			//InputTextCallback.ExecuteIfBound(callbackData);
			return 0;
		};

		static char buf[MAX_LENGTH];
		bool succeeded = ImGui::InputText(TCHAR_TO_ANSI(*label), buf, MAX_LENGTH, ImGuiInputTextFlags, nullptr, nullptr/*user_data*/);

		if (succeeded)
			OutBuf = buf;

		return succeeded;
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Input with Keyboard")
	static bool InputTextMultiline(const FString& label, UPARAM(ref) FString& OutBuf, FVector2D size = FVector2D(0, 0), int32 ImGuiInputTextFlags = 0/*, ImGuiInputTextCallback callback = NULL, void* user_data = NULL*/)
	{
		static char buf[MAX_LENGTH];
		bool succeeded = ImGui::InputTextMultiline(TCHAR_TO_ANSI(*label), buf, MAX_LENGTH, ToImVec2(size), ImGuiInputTextFlags, nullptr, nullptr);

		if (succeeded)
			OutBuf = buf;

		return succeeded;
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Input with Keyboard")
	static bool InputTextWithHint(const FString& label, UPARAM(ref) FString& OutHint, UPARAM(ref) FString& OutBuf, int32 ImGuiInputTextFlags = 0/*, ImGuiInputTextCallback callback = NULL, void* user_data = NULL*/)
	{
		static char hint[MAX_LENGTH];
		static char buf[MAX_LENGTH];
		bool succeeded = ImGui::InputTextWithHint(TCHAR_TO_ANSI(*label), hint, buf, MAX_LENGTH, ImGuiInputTextFlags, nullptr, nullptr);

		if (succeeded)
		{
			OutHint = hint;
			OutBuf = buf;
		}

		return succeeded;
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Input with Keyboard")
	static bool InputFloat(const FString& label, TArray<float>& v, float step = 0.0f, float step_fast = 0.0f, FString format = TEXT("%.3f"), int32 ImGuiInputTextFlags = 0)
	{
		return ImGui::InputFloat(TCHAR_TO_ANSI(*label), v.GetData(), step, step_fast, TCHAR_TO_ANSI(*format), ImGuiInputTextFlags);
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Input with Keyboard")
	static bool InputFloatSimple(const FString& label, TArray<float>& v, FString format = TEXT("%.3f"), int32 ImGuiInputTextFlags = 0)
	{
		return ImGui::InputScalarN(TCHAR_TO_ANSI(*label), ImGuiDataType_Float, v.GetData(), v.Num(), nullptr, nullptr, TCHAR_TO_ANSI(*format), ImGuiInputTextFlags);
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Input with Keyboard")
	static bool InputInt(const FString& label, TArray<int>& v, int step = 1, int step_fast = 100, int32 ImGuiInputTextFlags = 0)
	{
		return ImGui::InputInt(TCHAR_TO_ANSI(*label), v.GetData(), step, step_fast, ImGuiInputTextFlags);
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Input with Keyboard")
	static bool InputIntSimple(const FString& label, TArray<int>& v, FString format = TEXT("%d"), int32 ImGuiInputTextFlags = 0)
	{
		return ImGui::InputScalarN(TCHAR_TO_ANSI(*label), ImGuiDataType_S32, v.GetData(), v.Num(), nullptr, nullptr, TCHAR_TO_ANSI(*format), ImGuiInputTextFlags);
	}


	/*  Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.)  */


	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Color Editor/Picker")
	static bool ColorEdit(const FString& label, FLinearColor color, int32 ImGuiColorEditFlags = 0)
	{
		float col[4] = {color.R, color.G, color.B, color.A};
		return ImGui::ColorEdit4(TCHAR_TO_ANSI(*label), col, ImGuiColorEditFlags);
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Color Editor/Picker")
	static bool ColorPicker(const FString& label, FLinearColor color, int32 ImGuiColorEditFlags = 0)
	{
		float col[4] = {color.R, color.G, color.B, color.A};
		return ImGui::ColorPicker4(TCHAR_TO_ANSI(*label), col, ImGuiColorEditFlags);
	}

	// display a color square/button, hover for details, return true when pressed.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Color Editor/Picker")
	static bool ColorButton(const FString& desc_id, const FVector4& col, int32 ImGuiColorEditFlags = 0,
	                        FVector2D size = FVector2D(0, 0))
	{
		return ImGui::ColorButton(TCHAR_TO_ANSI(*desc_id), ToImVec4(col), ImGuiColorEditFlags, ToImVec2(size));
	}

	// initialize current options (generally on application startup) if you want to select a default format, picker type, etc. User will be able to change many settings, unless you pass the _NoOptions flag to your calls.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Color Editor/Picker")
	static void SetColorEditOptions(int32 ImGuiColorEditFlags) { ImGui::SetColorEditOptions(ImGuiColorEditFlags); }


	/* Widgets: Trees */

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Trees")
	static bool TreeNode(const FString& label) { return ImGui::TreeNode(TCHAR_TO_ANSI(*label)); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Trees")
	static bool TreeNodeEx(const FString& label, int32 ImGuiTreeNodeFlags = 0)
	{
		return ImGui::TreeNodeEx(TCHAR_TO_ANSI(*label), ImGuiTreeNodeFlags);
	}

	// ~ Indent()+PushId(). Already called by TreeNode() when returning true, but you can call TreePush/TreePop yourself if desired.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Trees")
	static void TreePush(const FString& str_id) { ImGui::TreeNodeEx(TCHAR_TO_ANSI(*str_id)); }

	// ~ Unindent()+PopId()
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Trees")
	static void TreePop() { ImGui::TreePop(); }

	// horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode
	UFUNCTION(BlueprintPure, Category = "ImGui|Widgets|Trees")
	static float GetTreeNodeToLabelSpacing() { return ImGui::GetTreeNodeToLabelSpacing(); }

	// // if returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop().
	// UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Trees")
	// static bool CollapsingHeader(const FString& label, ImGuiTreeNodeFlags flags = 0){ return ImGui::CollapsingHeader(TCHAR_TO_ANSI(*label), flags); }

	// when 'p_visible != NULL': if '*p_visible==true' display an additional small close button on upper right of the header which will set the bool to false when clicked, if '*p_visible==false' don't display the header.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Trees", meta = (ExpandEnumAsExecs="OutResult", AdvancedDisplay = "2"))
	static void CollapsingHeader(const FString& label, UPARAM(ref) bool& p_visible, TEnumAsByte<EImGuiFlowControl::Type>& OutResult, int32 ImGuiTreeNodeFlags = 0)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImGui::CollapsingHeader(TCHAR_TO_ANSI(*label), &p_visible, ImGuiTreeNodeFlags))
			OutResult = EImGuiFlowControl::Success;
	}

	// set next TreeNode/CollapsingHeader open state.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Trees")
	static void SetNextItemOpen(bool is_open, int32 ImGuiCond = 0) { ImGui::SetNextItemOpen(is_open, ImGuiCond); }


	/* Widgets: Selectables */


	// "bool* p_selected" point to the selection state (read-write), as a convenient helper.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Selectables")
	static bool Selectable(const FString& label, UPARAM(ref) bool& p_selected, int32 ImGuiSelectableFlags = 0,
	                       FVector2D size = FVector2D(0, 0))
	{
		return ImGui::Selectable(TCHAR_TO_ANSI(*label), &p_selected, ImGuiSelectableFlags, ToImVec2(size));
	}


	/* Widgets: List Boxes */


	// open a framed scrolling region
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|List Boxes")
	static bool BeginListBox(const FString& label, FVector2D size = FVector2D(0, 0))
	{
		return ImGui::BeginListBox(TCHAR_TO_ANSI(*label), ToImVec2(size));
	}

	// only call EndListBox() if BeginListBox() returned true!
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|List Boxes")
	static void EndListBox() { ImGui::EndListBox(); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|List Boxes")
	static bool ListBox(const FString& label, UPARAM(ref) int32& current_item, TArray<FString> items,
	                    int32 height_in_items = -1);

	/*UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|List Boxes")
	static bool ListBoxA(const FString& label, UPARAM(ref) int32& current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int32 items_count, int32 height_in_items = -1)
	{ return ImGui::ListBox(TCHAR_TO_ANSI(*label), &current_item, items_getter, data, items_count, height_in_items); }*/


	// Widgets: Data Plotting
	// - Consider using ImPlot (https://github.com/epezent/implot)


	//UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Data Plotting")
	static void PlotLines(const FString& label, TArray<float>& values, int values_count, int values_offset = 0,
	                      FString overlay_text = TEXT(""), float scale_min = FLT_MAX, float scale_max = FLT_MAX,
	                      ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float))
	{
		ImGui::PlotLines(TCHAR_TO_ANSI(*label), values.GetData(), values_count, values_offset,
		                 overlay_text.Len() > 0 ? TCHAR_TO_ANSI(*overlay_text) : nullptr, scale_min, scale_max,
		                 graph_size, stride);
	}

	//UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Data Plotting")
	//static void PlotLines(const FString& label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));

	//UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Data Plotting")
	//static void PlotHistogram(const FString& label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));

	//UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Data Plotting")
	//static void PlotHistogram(const FString& label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));


	/* Widgets: Value() Helpers. */


	// - Those are merely shortcut to calling Text() with a format string. Output single value in "name: value" format (tip: freely declare more in your code to handle your types. you can add functions to the ImGui namespace)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Value() Helpers")
	static void Value(const FString& prefix, bool b) { ImGui::Value(TCHAR_TO_ANSI(*prefix), b); }

	// - Those are merely shortcut to calling Text() with a format string. Output single value in "name: value" format (tip: freely declare more in your code to handle your types. you can add functions to the ImGui namespace)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Value() Helpers")
	static void ValueA(const FString& prefix, int32 v) { ImGui::Value(TCHAR_TO_ANSI(*prefix), v); }

	// - Those are merely shortcut to calling Text() with a format string. Output single value in "name: value" format (tip: freely declare more in your code to handle your types. you can add functions to the ImGui namespace)
	//UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Value() Helpers")
	static void ValueB(const FString& prefix, unsigned int v) { ImGui::Value(TCHAR_TO_ANSI(*prefix), v); }

	// - Those are merely shortcut to calling Text() with a format string. Output single value in "name: value" format (tip: freely declare more in your code to handle your types. you can add functions to the ImGui namespace)
	//UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Value() Helpers")
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Value() Helpers")
	static void ValueC(const FString& prefix, float v, const FString& float_format)
	{
		ImGui::Value(TCHAR_TO_ANSI(*prefix), v, float_format.Len() > 0 ? TCHAR_TO_ANSI(*float_format) : nullptr);
	}

	/** append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar flag set on parent window). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Menus", meta = (ExpandEnumAsExecs="OutResult"))
	static void BeginMenuBar(TEnumAsByte<EImGuiFlowControl::Type>& OutResult)
	{
		OutResult = ImGui::BeginMenuBar() ? EImGuiFlowControl::Success : EImGuiFlowControl::Failure;
	}

	/** only call EndMenuBar() if BeginMenuBar() returns true! */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Menus")
	static void EndMenuBar() { ImGui::EndMenuBar(); }

	/** create and append to a full screen menu-bar. */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Menus", meta = (ExpandEnumAsExecs="OutResult"))
	static void BeginMainMenuBar(TEnumAsByte<EImGuiFlowControl::Type>& OutResult)
	{
		OutResult = ImGui::BeginMainMenuBar() ? EImGuiFlowControl::Success : EImGuiFlowControl::Failure;
		if(OutResult == EImGuiFlowControl::Failure)
			ImGui::EndMenu();
	}

	/** only call EndMainMenuBar() if BeginMainMenuBar() returns true! */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Menus")
	static void EndMainMenuBar() { ImGui::EndMainMenuBar(); }

	/** create a sub-menu entry. only call EndMenu() if this returns true! */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Menus", meta = (ExpandEnumAsExecs="OutResult", AdvancedDisplay = "1"))
	static void BeginMenu(const FString& label, TEnumAsByte<EImGuiFlowControl::Type>& OutResult, bool enabled = true)
	{
		OutResult = ImGui::BeginMenu(TCHAR_TO_ANSI(*label), enabled) ? EImGuiFlowControl::Success : EImGuiFlowControl::Failure;
	}

	/** only call EndMenu() if BeginMenu() returns true! */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Menus")
	static void EndMenu() { ImGui::EndMenu(); }

	/** return true when activated. shortcuts are displayed for convenience but not processed by ImGui at the moment */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Widgets|Menus", meta = (ExpandEnumAsExecs="OutResult", AdvancedDisplay = "2"))
	static void MenuItem(const FString& label, UPARAM(ref) bool& selected, TEnumAsByte<EImGuiFlowControl::Type>& OutResult, FString shortcut = "", bool enabled = true)
	{
		OutResult = EImGuiFlowControl::Failure;			
		if(ImGui::MenuItem(TCHAR_TO_ANSI(*label), (shortcut.Len() > 0 ? TCHAR_TO_ANSI(*shortcut) : nullptr), &selected, enabled))
		{
			OutResult = EImGuiFlowControl::Success;	
		}
	}


	 /* Tooltips */

	/** begin/append a tooltip window. to create full-featured tooltip (with any kind of items). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tooltips")
	static void BeginTooltip() { ImGui::BeginTooltip(); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Tooltips")
	static void EndTooltip() { ImGui::EndTooltip(); }

	/** set a text-only tooltip, typically use with ImGui::IsItemHovered(). override any previous call to SetTooltip(). */
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tooltips")
	static void SetTooltip(const FString& print) { ImGui::SetTooltip(TCHAR_TO_ANSI(*print), nullptr); }


	
	 /* Popups, Modals */


	// return true if the popup is open, and you can start outputting to it.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups|Modals")
	static bool BeginPopup(const FString& str_id, int32 ImGuiWindowFlags = 0)
	{
		return ImGui::BeginPopup(TCHAR_TO_ANSI(*str_id), ImGuiWindowFlags);
	}
	
	// return true if the modal is open, and you can start outputting to it.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups|Modals")
	static bool BeginPopupModal(const FString& name, UPARAM(ref) bool& p_open, int32 ImGuiWindowFlags = 0)
	{
		return ImGui::BeginPopupModal(TCHAR_TO_ANSI(*name), &p_open, ImGuiWindowFlags);
	}

	// only call EndPopup() if BeginPopupXXX() returns true!
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups|Modals")
	static void EndPopup() { ImGui::EndPopup(); }


	 /* Popups: open/close functions */


	// call to mark popup as open (don't call every frame!).
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups|open/close")
	static void OpenPopup(const FString& str_id, int32 ImGuiPopupFlags = 0)
	{
		return ImGui::OpenPopup(TCHAR_TO_ANSI(*str_id), ImGuiPopupFlags);
	}

	// helper to open popup when clicked on last item. return true when just opened. (note: actually triggers on the mouse _released_ event to be consistent with popup behaviors)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups|open/close")
	static void OpenPopupOnItemClick(const FString& str_id, int32 ImGuiPopupFlags = 1)
	{
		return ImGui::OpenPopupOnItemClick(TCHAR_TO_ANSI(*str_id), ImGuiPopupFlags);
	}

	// manually close the popup we have begin-ed into.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups|open/close")
	static void CloseCurrentPopup(){ ImGui::CloseCurrentPopup(); }

	
	// Popups: open+begin combined functions helpers


	// open+begin popup when clicked on last item. if you can pass a NULL str_id only if the previous item had an id. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. read comments in .cpp!
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups|open+begin combined")
	static bool BeginPopupContextItem(const FString& str_id, int32 ImGuiPopupFlags = 1)
	{
		return ImGui::BeginPopupContextItem(TCHAR_TO_ANSI(*str_id), ImGuiPopupFlags);
	}

	// open+begin popup when clicked on current window.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups|open+begin combined")
	static bool BeginPopupContextWindow(const FString& str_id, int32 ImGuiPopupFlags = 1)
	{
		return ImGui::BeginPopupContextWindow(TCHAR_TO_ANSI(*str_id), ImGuiPopupFlags);
	}

	// open+begin popup when clicked in void (where there are no windows).
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups|open+begin combined")
	static bool BeginPopupContextVoid(const FString& str_id, int32 ImGuiPopupFlags = 1)
	{
		return ImGui::BeginPopupContextVoid(TCHAR_TO_ANSI(*str_id), ImGuiPopupFlags);
	}

	
	 /* Popups: test function */


	// return true if the popup is open.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Popups|test")
	static bool IsPopupOpen(const FString& str_id, int32 ImGuiPopupFlags = 0)
	{
		return ImGui::IsPopupOpen(TCHAR_TO_ANSI(*str_id), ImGuiPopupFlags);
	}
	


	/* Tables */

	
	
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables")
	static bool BeginTable(const FString& str_id, int column, int32 ImGuiTableFlags = 0, FVector2D outer_size = FVector2D(0.0f, 0.0f), float inner_width = 0.0f)
	{
		return ImGui::BeginTable(TCHAR_TO_ANSI(*str_id), column, ImGuiTableFlags, ToImVec2(outer_size), inner_width);
	}

	
	// only call EndTable() if BeginTable() returns true!
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables")
	static void EndTable()	{ ImGui::EndTable(); }

	// append into the first cell of a new row.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables")
	static void TableNextRow(int32 ImGuiTableRowFlags = 0, float min_row_height = 0.0f)
	{ ImGui::TableNextRow(ImGuiTableRowFlags, min_row_height); }

	// append into the next column (or first column of next row if currently in last column). Return true when column is visible.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables")
	static bool TableNextColumn() { return ImGui::TableNextColumn(); }

	// append into the specified column. Return true when column is visible.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables")
	static bool TableSetColumnIndex(int column_n) { return ImGui::TableSetColumnIndex(column_n); }
	


	 /*  Tables: Headers & Columns declaration  */

	

	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables|Headers & Columns")
	static void TableSetupColumn(const FString& label, int32 ImGuiTableColumnFlags = 0, float init_width_or_weight = 0.0f, int32 user_id = 0)
	{
		ImGui::TableSetupColumn(TCHAR_TO_ANSI(*label), ImGuiTableColumnFlags, init_width_or_weight, user_id);
	}

	// lock columns/rows so they stay visible when scrolled.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables|Headers & Columns")
	static void TableSetupScrollFreeze(int32 cols, int32 rows)
	{
		ImGui::TableSetupScrollFreeze(cols, rows);
	}
	
	// submit all headers cells based on data provided to TableSetupColumn() + submit context menu
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables|Headers & Columns")
	static void TableHeadersRow() { ImGui::TableHeadersRow(); }

	// submit one header cell manually (rarely used)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables|Headers & Columns")
	static void TableHeader(const FString& label){ ImGui::TableHeader(TCHAR_TO_ANSI(*label)); }


	// Tables: Sorting
	

	// get latest sort specs for the table (NULL if not sorting).
	//UFUNCTION(BlueprintCallable, Category = "ImGui|Tables|Sorting")
	//static ImGuiTableSortSpecs* TableGetSortSpecs() { return ImGui::TableGetSortSpecs(); }
	
	// Tables: Miscellaneous functions
	// - Functions args 'int column_n' treat the default value of -1 as the same as passing the current column index.

	// return number of columns (value passed to BeginTable)
	UFUNCTION(BlueprintPure, Category = "ImGui|Tables|Miscellaneous")
	static int32 TableGetColumnCount(){ return ImGui::TableGetColumnCount(); }

	// return current column index.
	UFUNCTION(BlueprintPure, Category = "ImGui|Tables|Miscellaneous")
	static int32 TableGetColumnIndex(){ return ImGui::TableGetColumnIndex(); }

	// return current row index.
	UFUNCTION(BlueprintPure, Category = "ImGui|Tables|Miscellaneous")
	static int32 TableGetRowIndex(){ return ImGui::TableGetRowIndex(); }

	// return "" if column didn't have a name declared by TableSetupColumn(). Pass -1 to use current column.
	UFUNCTION(BlueprintPure, Category = "ImGui|Tables|Miscellaneous")
	static FString TableGetColumnName(int32 column_n = -1) { return ANSI_TO_TCHAR(ImGui::TableGetColumnName(column_n)); }

	// return column flags so you can query their Enabled/Visible/Sorted/Hovered status flags. Pass -1 to use current column.
	UFUNCTION(BlueprintPure, Category = "ImGui|Tables|Miscellaneous")
	static int32 TableGetColumnFlags(int column_n = -1) { return /* ImGuiTableColumnFlags */ ImGui::TableGetColumnFlags(column_n); }

	// change the color of a cell, row, or column. See ImGuiTableBgTarget_ flags for details.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables|Miscellaneous")
	static void TableSetBgColor(int32 ImGuiTableBgTarget, int32 color, int column_n = -1)
	{ ImGui::TableSetBgColor(ImGuiTableBgTarget, color, column_n); }
	


	 /* Legacy Columns API (2020: prefer using Tables!)
	 - You can also use SameLine(pos_x) to mimic simplified columns. */

	
	
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables|Legacy Columns API")
	static void Columns(int count = 1, FString id = TEXT(""), bool border = true)
	{ 
		ImGui::Columns(count, TCHAR_TO_ANSI(*id), border); 
	}

	// next column, defaults to current row or next row if the current row is finished
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables|Legacy Columns API")
	static void NextColumn() { ImGui::NextColumn(); }

	// get current column index
	UFUNCTION(BlueprintPure, Category = "ImGui|Tables|Legacy Columns API")
	static int32 GetColumnIndex() { return ImGui::GetColumnIndex(); }

	// get column width (in pixels). pass -1 to use current column
	UFUNCTION(BlueprintPure, Category = "ImGui|Tables|Legacy Columns API")
	static float GetColumnWidth(int32 column_index = -1) { return ImGui::GetColumnWidth(column_index); }

	// set column width (in pixels). pass -1 to use current column
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables|Legacy Columns API")
	static void SetColumnWidth(int32 column_index, float width){ ImGui::SetColumnWidth(column_index, width); }

	// get position of column line (in pixels, from the left side of the contents region). pass -1 to use current column, otherwise 0..GetColumnsCount() inclusive. column 0 is typically 0.0f
	UFUNCTION(BlueprintPure, Category = "ImGui|Tables|Legacy Columns API")
	static float GetColumnOffset(int32 column_index = -1) { return ImGui::GetColumnOffset(column_index); }

	// set position of column line (in pixels, from the left side of the contents region). pass -1 to use current column
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tables|Legacy Columns API")
	static void SetColumnOffset(int32 column_index, float offset_x) { ImGui::SetColumnOffset(column_index, offset_x); }

	UFUNCTION(BlueprintPure, Category = "ImGui|Tables|Legacy Columns API")
	static int32 GetColumnsCount(){ return ImGui::GetColumnsCount(); }
	
	 /* Tab Bars, Tabs */


	// create and append into a TabBar
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tab Bars|Tabs")
	static bool BeginTabBar(const FString& str_id, int32 ImGuiTabBarFlags = 0)
	{ return ImGui::BeginTabBar(TCHAR_TO_ANSI(*str_id), ImGuiTabBarFlags); }

	// only call EndTabBar() if BeginTabBar() returns true!
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tab Bars|Tabs")
	static void EndTabBar() { ImGui::EndTabBar(); }

	// create a Tab. Returns true if the Tab is selected.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tab Bars|Tabs")
	static bool BeginTabItem(const FString& label, UPARAM(ref) bool& p_open, int32 ImGuiTabItemFlags = 0)
	{ return ImGui::BeginTabItem(TCHAR_TO_ANSI(*label), &p_open, ImGuiTabItemFlags); }

	// only call EndTabItem() if BeginTabItem() returns true!
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tab Bars|Tabs")
	static void EndTabItem() { ImGui::EndTabItem(); }

	// create a Tab behaving like a button. return true when clicked. cannot be selected in the tab bar.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tab Bars|Tabs")
	static bool TabItemButton(const FString& label, int32 ImGuiTabItemFlags = 0)
	{ return ImGui::TabItemButton(TCHAR_TO_ANSI(*label), ImGuiTabItemFlags); }

	// notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Tab Bars|Tabs")
	static void SetTabItemClosed(const FString& tab_or_docked_window_label)
	{ ImGui::SetTabItemClosed(TCHAR_TO_ANSI(*tab_or_docked_window_label)); }
	

	 /* Logging/Capture  */


	
	// start logging to tty (stdout)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Logging/Capture")
	static void LogToTTY(int32 auto_open_depth = -1) { ImGui::LogToTTY(auto_open_depth); }

	// start logging to file
	UFUNCTION(BlueprintCallable, Category = "ImGui|Logging/Capture")
	static void LogToFile(const FString& filename, int32 auto_open_depth = -1)
	{ ImGui::LogToFile(auto_open_depth, TCHAR_TO_ANSI(*filename)); }

	// start logging to OS clipboard
	UFUNCTION(BlueprintCallable, Category = "ImGui|Logging/Capture")
	static void LogToClipboard(int32 auto_open_depth = -1) { ImGui::LogToClipboard(auto_open_depth); }

	// stop logging (close file, etc.)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Logging/Capture")
	static void LogFinish(){ ImGui::LogFinish(); }

	// helper to display buttons for logging to tty/file/clipboard
	UFUNCTION(BlueprintCallable, Category = "ImGui|Logging/Capture")
	static void LogButtons(){ ImGui::LogButtons(); }

	// pass text data straight to log (without being displayed)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Logging/Capture")
	static void LogText(const FString& fmt) { ImGui::LogText(TCHAR_TO_ANSI(*fmt), nullptr); }
	
	
	 /* Drag and Drop */
	

	// call when the current item is active. If this return true, you can call SetDragDropPayload() + EndDragDropSource()
	UFUNCTION(BlueprintCallable, Category = "ImGui|Drag and Drop")
	static bool BeginDragDropSource(int32 ImGuiDragDropFlags = 0){ return ImGui::BeginDragDropSource(ImGuiDragDropFlags); }

	// type is a user defined string of maximum 32 characters. Strings starting with '_' are reserved for dear imgui internal types. Data is copied and held by imgui.
	//UFUNCTION(BlueprintCallable, Category = "ImGui|Drag and Drop")
	static bool SetDragDropPayload(const FString& type, const void* data, size_t sz, int32 ImGuiCond = 0)
	{ return ImGui::SetDragDropPayload(TCHAR_TO_ANSI(*type), data, sz, ImGuiCond); }

	// only call EndDragDropSource() if BeginDragDropSource() returns true!
	UFUNCTION(BlueprintCallable, Category = "ImGui|Drag and Drop")
	static void EndDragDropSource() { ImGui::EndDragDropSource(); }

	// call after submitting an item that may receive a payload. If this returns true, you can call AcceptDragDropPayload() + EndDragDropTarget()
	UFUNCTION(BlueprintCallable, Category = "ImGui|Drag and Drop")
	static bool BeginDragDropTarget() { return ImGui::BeginDragDropTarget(); }

	// accept contents of a given type. If ImGuiDragDropFlags_AcceptBeforeDelivery is set you can peek into the payload before the mouse button is released.
	//UFUNCTION(BlueprintCallable, Category = "ImGui|Drag and Drop")
	static const ImGuiPayload* AcceptDragDropPayload(const FString& type, int32 ImGuiDragDropFlags = 0)
	{ return ImGui::AcceptDragDropPayload(TCHAR_TO_ANSI(*type), ImGuiDragDropFlags); }

	// only call EndDragDropTarget() if BeginDragDropTarget() returns true!
	UFUNCTION(BlueprintCallable, Category = "ImGui|Drag and Drop")
	static void EndDragDropTarget(){ ImGui::EndDragDropTarget(); }

	// peek directly into the current payload from anywhere. may return NULL. use ImGuiPayload::IsDataType() to test for the payload type.
	//UFUNCTION(BlueprintCallable, Category = "ImGui|Drag and Drop")
	static const ImGuiPayload*  GetDragDropPayload() { return ImGui::GetDragDropPayload(); }
	

	/* Clipping */


	UFUNCTION(BlueprintCallable, Category = "ImGui|Clipping")
	static void PushClipRect(const FVector2D& clip_rect_min, const FVector2D& clip_rect_max, bool intersect_with_current_clip_rect)
	{ ImGui::PushClipRect(ToImVec2(clip_rect_min), ToImVec2(clip_rect_max), intersect_with_current_clip_rect); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Clipping")
	static void PopClipRect() { ImGui::PopClipRect(); }
	

	/*  Focus, Activation */
	

	// make last item the default focused item of a window.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Focus, Activation")
	static void SetItemDefaultFocus() { ImGui::SetItemDefaultFocus(); }

	// focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Focus, Activation")
	static void SetKeyboardFocusHere(int32 offset = 0) { ImGui::SetKeyboardFocusHere(offset); }
	


	/* Item/Widgets Utilities  */
	

	
	// is the last item hovered? (and usable, aka not blocked by a popup, etc.). See ImGuiHoveredFlags for more options.
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsItemHovered(int32 ImGuiHoveredFlags = 0)
	{ return ImGui::IsItemHovered(ImGuiHoveredFlags); }

	// is the last item active? (e.g. button being held, text field being edited. This will continuously return true while holding mouse button on an item. Items that don't interact will always return false)
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsItemActive() { return ImGui::IsItemActive(); }

	// is the last item focused for keyboard/gamepad navigation?
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsItemFocused() { return ImGui::IsItemFocused(); }

	// is the last item clicked? (e.g. button/node just clicked on) == IsMouseClicked(mouse_button) && IsItemHovered()
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsItemClicked(int32 ImGuiMouseButton = 0) { return ImGui::IsItemClicked(ImGuiMouseButton); }

	// is the last item visible? (items may be out of sight because of clipping/scrolling)
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsItemVisible() { return ImGui::IsItemVisible(); }

	// did the last item modify its underlying value this frame? or was pressed? This is generally the same as the "bool" return value of many widgets.
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsItemEdited(){ return ImGui::IsItemEdited(); }

	// was the last item just made active (item was previously inactive).
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsItemActivated(){ return ImGui::IsItemActivated(); }

	// was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that requires continuous editing.
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsItemDeactivated(){ return ImGui::IsItemDeactivated(); }

	// was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that requires continuous editing. Note that you may get false positives (some widgets such as Combo()/ListBox()/Selectable() will return true even when clicking an already selected item).
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsItemDeactivatedAfterEdit(){ return ImGui::IsItemDeactivatedAfterEdit(); }

	// was the last item open state toggled? set by TreeNode().
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsItemToggledOpen(){ return ImGui::IsItemToggledOpen(); }

	// is any item hovered?
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsAnyItemHovered(){ return ImGui::IsAnyItemHovered(); }

	// is any item active?
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsAnyItemActive(){ return ImGui::IsAnyItemActive(); }

	// is any item focused?
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities")
	static bool IsAnyItemFocused(){ return ImGui::IsAnyItemFocused(); }

	// get upper-left bounding rectangle of the last item (screen space)
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities", Meta = (ReturnDisplayName = "UpperLeft"))
	static FVector2D GetItemRectMin(){ return ToVector2D(ImGui::GetItemRectMin()); }

	// get lower-right bounding rectangle of the last item (screen space)
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities", Meta = (ReturnDisplayName = "LowerRight"))
	static FVector2D GetItemRectMax(){ return ToVector2D(ImGui::GetItemRectMax()); }

	// get size of last item
	UFUNCTION(BlueprintPure, Category = "ImGui|Item/Widgets Utilities", Meta = (ReturnDisplayName = "Size"))
	static FVector2D GetItemRectSize(){ return ToVector2D(ImGui::GetItemRectSize()); }

	// allow last item to be overlapped by a subsequent item. sometimes useful with invisible buttons, selectables, etc. to catch unused area.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Item/Widgets Utilities")
	static void SetItemAllowOverlap() { ImGui::SetItemAllowOverlap(); }

	
	
	 /* Viewports */


	// return primary/default viewport.
	//UFUNCTION(BlueprintPure, Category = "ImGui|Viewports")
	static ImGuiViewport* GetMainViewport() { return ImGui::GetMainViewport(); }
	
	
	// test if rectangle (of given size, starting from cursor position) is visible / not clipped.
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities")
	static bool IsRectVisible(const FVector2D& size) { return ImGui::IsRectVisible(ToImVec2(size)); }

	// test if rectangle (in screen space) is visible / not clipped. to perform coarse clipping on user's side.
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities")
	static bool IsRectVisibleA(const FVector2D& rect_min, const FVector2D& rect_max)
	{ return ImGui::IsRectVisible(ToImVec2(rect_min), ToImVec2(rect_max)); }

	// get global imgui time. incremented by io.DeltaTime every frame.
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities")
	static float GetTime() { return ImGui::GetTime(); }

	// get global imgui frame count. incremented by 1 every frame.
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities")
	static int32 GetFrameCount() { return ImGui::GetFrameCount(); }

	// this draw list will be the first rendering one. Useful to quickly draw shapes/text behind dear imgui contents.
	//UFUNCTION(BlueprintPure, Category = "ImGui|Utilities")
	static ImDrawList* GetBackgroundDrawList() { return ImGui::GetBackgroundDrawList(); }

	// this draw list will be the last rendered one. Useful to quickly draw shapes/text over dear imgui contents.
	//UFUNCTION(BlueprintPure, Category = "ImGui|Utilities")
	static ImDrawList* GetForegroundDrawList() { return ImGui::GetForegroundDrawList(); }

	// you may use this when creating your own ImDrawList instances.
	//UFUNCTION(BlueprintPure, Category = "ImGui|Utilities")
	static ImDrawListSharedData* GetDrawListSharedData() { return ImGui::GetDrawListSharedData(); }

	// get a string corresponding to the enum value (for display, saving, etc.).
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities")
	static FString GetStyleColorName(int32 ImGuiCol) { return ANSI_TO_TCHAR(ImGui::GetStyleColorName(ImGuiCol)); }

	// replace current window storage with our own (if you want to manipulate it yourself, typically clear subsection of it)
	//UFUNCTION(BlueprintPure, Category = "ImGui|Utilities")
	static void SetStateStorage(ImGuiStorage* storage) { ImGui::SetStateStorage(storage); }

	//UFUNCTION(BlueprintPure, Category = "ImGui|Utilities")
	static ImGuiStorage* GetStateStorage() { return ImGui::GetStateStorage(); }

	// calculate coarse clipping for large list of evenly sized items. Prefer using the ImGuiListClipper higher-level helper if you can.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities")
	static void CalcListClipping(int32 items_count, float items_height, UPARAM(ref) int& out_items_display_start, UPARAM(ref) int& out_items_display_end)
	{ ImGui::CalcListClipping(items_count, items_height, &out_items_display_start, &out_items_display_end); }

	// helper to create a child window / scrolling region that looks like a normal widget frame
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities")
	static bool BeginChildFrame(int32 ImGuiID, const FVector2D& size, int32 ImGuiWindowFlags = 0)
	{ return ImGui::BeginChildFrame(ImGuiID, ToImVec2(size), ImGuiWindowFlags); }

	// always call EndChildFrame() regardless of BeginChildFrame() return values (which indicates a collapsed/clipped window)
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities")
	static void EndChildFrame() { ImGui::EndChildFrame(); }

	
	
	// Text Utilities
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Text")
	static FVector2D CalcTextSize(const FString& text, FString text_end, bool hide_text_after_double_hash = false, float wrap_width = -1.0f)
	{
		return ToVector2D(ImGui::CalcTextSize(TCHAR_TO_ANSI(*text), text_end.Len() > 0 ? TCHAR_TO_ANSI(*text_end) : nullptr, hide_text_after_double_hash, wrap_width));
	}
	
	// Color Utilities
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Color")
	static FLinearColor ColorConvertU32ToFloat4(int32 in)
	{
		return ToLinearColor(ImGui::ColorConvertU32ToFloat4(in));
	}

	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Color")
	static int32 ColorConvertFloat4ToU32(const FLinearColor& in)
	{
		return ImGui::ColorConvertFloat4ToU32(ToImVec4(in));
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Color")
	static void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
	{
		return ImGui::ColorConvertRGBtoHSV(r, g, b, out_h, out_s, out_v);
	}

	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Color")
	static void ColorConvertHSVtoRGB(float h, float s, float v, UPARAM(ref) float& out_r, UPARAM(ref) float& out_g, UPARAM(ref) float& out_b)
	{
		return ImGui::ColorConvertHSVtoRGB(h, s, v, out_r, out_g, out_b);
	}
	
	 /* Inputs Utilities: Keyboard */
	

	// map ImGuiKey_* values into user's key index. == io.KeyMap[key]
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Keyboard")
	static int32 GetKeyIndex(int32 ImGuiKey) { return ImGui::GetKeyIndex(ImGuiKey); }

	// is key being held. == io.KeysDown[user_key_index].
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Keyboard")
	static bool IsKeyDown(int32 user_key_index) { return ImGui::IsKeyDown(user_key_index); }

	// was key pressed (went from !Down to Down)? if repeat=true, uses io.KeyRepeatDelay / KeyRepeatRate
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Keyboard")
	static bool IsKeyPressed(int32 user_key_index, bool repeat = true)
	{ return ImGui::IsKeyPressed(user_key_index, repeat); }

	// was key released (went from Down to !Down)?
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Keyboard")
	static bool IsKeyReleased(int32 user_key_index) { return ImGui::IsKeyReleased(user_key_index); }

	// uses provided repeat rate/delay. return a count, most often 0 or 1 but might be >1 if RepeatRate is small enough that DeltaTime > RepeatRate
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Keyboard")
	static int32 GetKeyPressedAmount(int32 key_index, float repeat_delay, float rate)
	{ return ImGui::GetKeyPressedAmount(key_index, repeat_delay, rate); }

	// attention: misleading name! manually override io.WantCaptureKeyboard flag next frame (said flag is entirely left for your application to handle). e.g. force capture keyboard when your widget is being hovered. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard_value"; after the next NewFrame() call.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Keyboard")
	static void CaptureKeyboardFromApp(bool want_capture_keyboard_value = true)
	{ ImGui::CaptureKeyboardFromApp(want_capture_keyboard_value); }
	


	// Inputs Utilities: Mouse

	

	// is mouse button held?
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse")
	static bool IsMouseDown(int32 ImGuiMouseButton)	{ return ImGui::IsMouseDown(ImGuiMouseButton); }

	// did mouse button clicked? (went from !Down to Down)
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse")
	static bool IsMouseClicked(int32 ImGuiMouseButton, bool repeat = false)
	{ return ImGui::IsMouseClicked(ImGuiMouseButton, repeat); }

	// did mouse button released? (went from Down to !Down)
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse")
	static bool IsMouseReleased(int32 ImGuiMouseButton)
	{ return ImGui::IsMouseReleased(ImGuiMouseButton); }

	// did mouse button double-clicked? (note that a double-click will also report IsMouseClicked() == true)
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse")
	static bool IsMouseDoubleClicked(int32 ImGuiMouseButton)
	{ return ImGui::IsMouseDoubleClicked(ImGuiMouseButton); }

	// is mouse hovering given bounding rect (in screen space). clipped by current clipping settings, but disregarding of other consideration of focus/window ordering/popup-block.
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse")
	static bool IsMouseHoveringRect(const FVector2D& r_min, const FVector2D& r_max, bool clip = true)
	{ return ImGui::IsMouseHoveringRect(ToImVec2(r_min), ToImVec2(r_max), clip); }

	// by convention we use (-FLT_MAX,-FLT_MAX) to denote that there is no mouse available
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse")
	static bool IsMousePosValid(FVector2D& mouse_pos)
	{
		ImVec2 pos;
		bool succeeded = ImGui::IsMousePosValid(&pos);
		mouse_pos.X = pos.x;
		mouse_pos.Y = pos.y;
		return succeeded;
	}

	// is any mouse button held?
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse")
	static bool IsAnyMouseDown() { return ImGui::IsAnyMouseDown(); }

	// shortcut to ImGui::GetIO().MousePos provided by user, to be consistent with other calls
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse", Meta = (ReturnDisplayName = "Pos"))
	static FVector2D GetMousePos()  { return ToVector2D(ImGui::GetMousePos()); }

	// retrieve mouse position at the time of opening popup we have BeginPopup() into (helper to avoid user backing that value themselves)
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse", Meta = (ReturnDisplayName = "Pos"))
	static FVector2D GetMousePosOnOpeningCurrentPopup() { return ToVector2D(ImGui::GetMousePosOnOpeningCurrentPopup()); }

	// is mouse dragging? (if lock_threshold < -1.0f, uses io.MouseDraggingThreshold)
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse")
	static bool IsMouseDragging(int32 ImGuiMouseButton, float lock_threshold = -1.0f)
	{ return ImGui::IsMouseDragging(ImGuiMouseButton, lock_threshold); }

	// return the delta from the initial clicking position while the mouse button is pressed or was just released. This is locked and return 0.0f until the mouse moves past a distance threshold at least once (if lock_threshold < -1.0f, uses io.MouseDraggingThreshold)
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse", Meta = (ReturnDisplayName = "ClickingPos"))
	static FVector2D GetMouseDragDelta(int32 ImGuiMouseButton = 0, float lock_threshold = -1.0f)
	{ return ToVector2D(ImGui::GetMouseDragDelta(ImGuiMouseButton, lock_threshold)); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Mouse")
	static void ResetMouseDragDelta(int32 ImGuiMouseButton = 0)
	{ return ImGui::ResetMouseDragDelta(ImGuiMouseButton); }

	// get desired cursor type, reset in ImGui::NewFrame(), this is updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor ImGui will render those for you
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Mouse")
	static int32 GetMouseCursor()
	{ return /*ImGuiMouseCursor*/ ImGui::GetMouseCursor(); }
	

	// set desired cursor type
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Mouse")
	static void SetMouseCursor(int32 ImGuiMouseCursor)
	{ ImGui::SetMouseCursor(ImGuiMouseCursor); }

	// attention: misleading name! manually override io.WantCaptureMouse flag next frame (said flag is entirely left for your application to handle). This is equivalent to setting "io.WantCaptureMouse = want_capture_mouse_value;" after the next NewFrame() call.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Mouse")
	static void CaptureMouseFromApp(bool want_capture_mouse_value = true)
	{ ImGui::CaptureMouseFromApp(want_capture_mouse_value); }


	
	 /*  Clipboard Utilities  */

	
	// - Also see the LogToClipboard() function to capture GUI into clipboard, or easily output text data to the clipboard.
	UFUNCTION(BlueprintPure, Category = "ImGui|Utilities|Clipboard")
	static FString GetClipboardText() { return ANSI_TO_TCHAR(ImGui::GetClipboardText()); }

	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Clipboard")
	static void SetClipboardText(const FString& text)
	{ ImGui::SetClipboardText(TCHAR_TO_ANSI(*text)); }


	
	 /*  Settings/.Ini Utilities  */
	
	

	// call after CreateContext() and before the first call to NewFrame(). NewFrame() automatically calls LoadIniSettingsFromDisk(io.IniFilename).
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Clipboard")
	static void LoadIniSettingsFromDisk(const FString& ini_filename)
	{ ImGui::LoadIniSettingsFromDisk(TCHAR_TO_ANSI(*ini_filename)); }

	// call after CreateContext() and before the first call to NewFrame() to provide .ini data from your own data source.
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Clipboard")
	static void LoadIniSettingsFromMemory(const FString& ini_data, int32 ini_size = 0)
	{ ImGui::LoadIniSettingsFromMemory(TCHAR_TO_ANSI(*ini_data), ini_size); }

	// this is automatically called (if io.IniFilename is not empty) a few seconds after any modification that should be reflected in the .ini file (and also by DestroyContext).
	UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Clipboard")
	static void SaveIniSettingsToDisk(const FString& ini_filename)
	{ ImGui::SaveIniSettingsToDisk(TCHAR_TO_ANSI(*ini_filename)); }

	// return a zero-terminated string with the .ini data which you can save by your own mean. call when io.WantSaveIniSettings is set, then save data by your own mean and clear io.WantSaveIniSettings.
	//UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Clipboard")
	static FString SaveIniSettingsToMemory(size_t* out_ini_size = NULL)
	{ return ANSI_TO_TCHAR(ImGui::SaveIniSettingsToMemory(out_ini_size)); }
	


	 /*  Debug Utilities  */
	


	// This is called by IMGUI_CHECKVERSION() macro.
	//UFUNCTION(BlueprintCallable, Category = "ImGui|Utilities|Debug")
	static bool DebugCheckVersionAndDataLayout(const char* version_str, size_t sz_io, size_t sz_style, size_t sz_vec2, size_t sz_vec4, size_t sz_drawvert, size_t sz_drawidx);

	
	 /*  Memory Allocators  */



	static void SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void (*free_func)(void* ptr, void* user_data), void* user_data = NULL)
	{ return ImGui::SetAllocatorFunctions(alloc_func, free_func, user_data); }
	
	static void* MemAlloc(size_t size) { return ImGui::MemAlloc(size); }
	
	static void MemFree(void* ptr) { ImGui::MemFree(ptr); }
};
