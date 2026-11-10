#include "NumberFactoryPinFactory.h"

#include "NumberFactoryGraphSchema.h"

//const FName UAnimationStateMachineSchema::PC_Exec(TEXT("exec"));

TSharedPtr<class SGraphPin> FNumberFactoryPinFactory::CreatePin(class UEdGraphPin* Pin) const
{
	if (Pin->GetSchema()->IsA<UNumberFactoryGraphSchema>() && Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
	{
		return SNew(SGraphPinExec, Pin);
	}

	return nullptr;
}
