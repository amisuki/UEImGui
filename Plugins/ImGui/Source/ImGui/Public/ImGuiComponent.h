// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "CoreMinimal.h"

#include "ImGuiWrapperFunctionLibrary.h"
#include "Components/SceneComponent.h"
#include "ImGuiComponent.generated.h"


//UCLASS(DefaultToInstanced, BlueprintType, ClassGroup = Imgui)
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class IMGUI_API UImGuiComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UImGuiComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	FDelegateHandle IMGUI_TickHandle;
	void ImGuiTick();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "ImGui Tick"))
	void ReceiveImGuiTick();
	
protected:
	class FImGuiModule* Module = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FStackUnwinding stackTrace;
};
