// Distributed under the MIT License (MIT) (see accompanying LICENSE file)


#include "ImplotWrapperFunctionLibrary.h"

static TArray<TArray<float>> TwoDimensionalArray;
const FLinearColor UImplotWrapperFunctionLibrary::UE_IMPLOT_AUTO_COL(0.f,0.f,0.f, -1.0f);


/* Example function for parsing a single property
* @param Property    the property reflection data
* @param ValuePtr    the pointer to the property value
*/
void ParseProperty(FProperty* Property, void* ValuePtr)
{        
        
    float FloatValue;
    int32 IntValue;
    bool BoolValue;
    FString StringValue;
    FName NameValue;
    FText TextValue;
 
 
    // Here's how to read integer and float properties
    if (FNumericProperty *NumericProperty = Cast<FNumericProperty>(Property))
    {
        if (NumericProperty->IsFloatingPoint())
        {
            FloatValue = NumericProperty->GetFloatingPointPropertyValue(ValuePtr);
        }
        else if (NumericProperty->IsInteger())
        {
            IntValue = NumericProperty->GetSignedIntPropertyValue(ValuePtr);
        }
    }
 
    // How to read booleans
    if (FBoolProperty* BoolProperty = Cast<FBoolProperty>(Property))
    {
        BoolValue = BoolProperty->GetPropertyValue(ValuePtr);
    }
 
    // Reading names
    if (FNameProperty* NameProperty = Cast<FNameProperty>(Property))
    {
        NameValue = NameProperty->GetPropertyValue(ValuePtr);
    }
 
    // Reading strings
    if (FStrProperty* StringProperty = Cast<FStrProperty>(Property))
    {
        StringValue = StringProperty->GetPropertyValue(ValuePtr);
    }
 
    // Reading texts
    if (FTextProperty* TextProperty = Cast<FTextProperty>(Property))
    {
        TextValue = TextProperty->GetPropertyValue(ValuePtr);
    }
 
    // Reading an array
    if (FArrayProperty* ArrayProperty = Cast<FArrayProperty>(Property))
    {
        // We need the helper to get to the items of the array            
        FScriptArrayHelper Helper(ArrayProperty, ValuePtr);
        for (int32 i = 0, n = Helper.Num(); i<n; ++i)
        {    
            ParseProperty(ArrayProperty->Inner, Helper.GetRawPtr(i));
        }
    }

}


void UImplotWrapperFunctionLibrary::Array_Shuffle2(const TArray<int32>& TargetArray)
{
    // We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
    check(0);
}

//
// void UImplotWrapperFunctionLibrary::ReceiveSomeStruct(FProperty* AnyStruct)
// {
//      void* aa = nullptr;
//      ParseProperty(AnyStruct,aa); 
//
// }
//  
void UImplotWrapperFunctionLibrary::GenericArray_Shuffle2(void* TargetArray, const FArrayProperty* ArrayProp)
{
    if (TargetArray)
    {
        FProperty* InnerProp = ArrayProp->Inner;
        const FBoolProperty* BoolProperty = CastField<const FBoolProperty>(InnerProp);
        if (BoolProperty)
        {
            ensure((BoolProperty->ElementSize * BoolProperty->ArrayDim) == sizeof(uint8));
            UE_LOG(LogTemp, Log, TEXT("BoolPropertyBoolPropertyBoolPropertyBoolPropertyBoolProperty"));
            //BoolProperty->SetPropertyValue(ItemPtr, 0 != *(reinterpret_cast<uint8*>(ItemPtr)));
        }
        
        FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
        int32 LastIndex = ArrayHelper.Num() - 1;
        for (int32 i = 0; i <= LastIndex; ++i)
        {            
            int32 Index = FMath::RandRange(i, LastIndex);
            if (i != Index)
            {
                ArrayHelper.SwapValues(i, Index);
            }
        }
    }
}
