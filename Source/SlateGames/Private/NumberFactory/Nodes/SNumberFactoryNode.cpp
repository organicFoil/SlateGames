// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SNumberFactoryNode.h"

#include "NumberFactoryNode.h"
#include "SlateOptMacros.h"
#include "Styling/SlateStyleMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNumberFactoryNode::Construct(const FArguments& InArgs, UNumberFactoryNode* InNode)
{
    GraphNode = InNode;
    UpdateGraphNode();
}

void SNumberFactoryNode::UpdateGraphNode()
{
    ensure(GraphNode);
    SGraphNode::UpdateGraphNode();
}

TSharedRef<SWidget> SNumberFactoryNode::CreateNodeContentArea()
{
    TSharedRef<SWidget> MainContainer = SNew(SBorder)
        .BorderImage(FAppStyle::GetBrush("NoBorder"))
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        .Padding(FMargin(0, 3))
        [
            SNew(SHorizontalBox)
            
            // LEFT
            + SHorizontalBox::Slot()
            .AutoWidth()
            .HAlign(HAlign_Left)
            [
                SAssignNew(LeftNodeBox, SVerticalBox)
            ]
            
            // CENTER
            + SHorizontalBox::Slot()
            .AutoWidth()
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Center)
            .Padding(0, 10.f)
            [
                SNew(STextBlock)
                .MinDesiredWidth(130.f)
                .Text_Lambda([this]()
                {
                    if (UNumberFactoryNode* NumberNode = Cast<UNumberFactoryNode>(GraphNode))
                    {
                        return NumberNode->CosmeticNodeValue;
                    }
                    return FText::GetEmpty();
                })
                .Justification(ETextJustify::Center)
                .Font(DEFAULT_FONT("Regular", 24))
            ]
            
            // RIGHT
            + SHorizontalBox::Slot()
            .AutoWidth()
            .HAlign(HAlign_Right)
            [
                SAssignNew(RightNodeBox, SVerticalBox)
            ]
        ];
    
    return MainContainer;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
