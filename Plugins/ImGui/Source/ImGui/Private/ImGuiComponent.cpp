// Distributed under the MIT License (MIT) (see accompanying LICENSE file)


#include "ImGuiComponent.h"

#include "ImGuiDelegates.h"
#include "ImGuiModule.h"

// Sets default values for this component's properties
UImGuiComponent::UImGuiComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UImGuiComponent::BeginPlay()
{
	Super::BeginPlay();

	IMGUI_TickHandle = FImGuiDelegates::OnWorldDebug().AddUObject(this, &UImGuiComponent::ImGuiTick);
	Module = &FImGuiModule::Get();
	
}

void UImGuiComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FImGuiDelegates::OnWorldDebug().Remove(IMGUI_TickHandle);
}


// Called every frame
void UImGuiComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UImGuiComponent::ImGuiTick()
{
	if (!Module->GetProperties().IsInputEnabled())
		return;
	
	ReceiveImGuiTick();

	stackTrace.Excute();
	stackTrace.Clear();
}

