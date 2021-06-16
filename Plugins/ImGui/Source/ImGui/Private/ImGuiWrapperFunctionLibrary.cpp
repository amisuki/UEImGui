// Distributed under the MIT License (MIT) (see accompanying LICENSE file)


#include "ImGuiWrapperFunctionLibrary.h"

bool UImGuiWrapperFunctionLibrary::Combo(const FString& label, int32& current_item, const TArray<FString>& items, int items_count, int popup_max_height_in_items)
{
	ConvertArrayFStringToArrayAnsi itemList(items);
	return ImGui::Combo(TCHAR_TO_ANSI(*label), &current_item, itemList.GetData(), items_count, popup_max_height_in_items);;
}

bool UImGuiWrapperFunctionLibrary::ListBox(const FString& label, int32& current_item, TArray<FString> items, int32 height_in_items)
{
	ConvertArrayFStringToArrayAnsi itemList(items);
	return ImGui::ListBox(TCHAR_TO_ANSI(*label), &current_item, itemList.GetData(), items.Num(), height_in_items);
}
