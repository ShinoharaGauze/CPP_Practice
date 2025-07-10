// Fill out your copyright notice in the Description page of Project Settings.

#include "CPAction.h"
#include "CPActionComponent.h"
#include "CPP_Practice/CPP_Practice.h"
#include "Net/UnrealNetwork.h"

// ====== 初始化函数 ======

/**
 * 初始化 Action，绑定所属的 ActionComponent。
 */
void UCPAction::Initialize(UCPActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

// ====== 动作逻辑 ======

/**
 * 判断是否可以启动此 Action。
 * 会检查是否已经运行中，以及是否被 BlockedTags 阻止。
 */
bool UCPAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UCPActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

/**
 * 启动 Action：
 * - 添加授予的 Tags；
 * - 设置 bIsRunning；
 * - 显示调试信息。
 */
void UCPAction::StartAction_Implementation(AActor* Instigator)
{
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UCPActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

/**
 * 停止 Action：
 * - 移除授予的 Tags；
 * - 设置 bIsRunning 为 false；
 * - 显示调试信息。
 */
void UCPAction::StopAction_Implementation(AActor* Instigator)
{
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	UCPActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

/**
 * 返回当前 Action 是否正在运行。
 */
bool UCPAction::IsRunning() const
{
	return RepData.bIsRunning;
}

// ====== 辅助函数 ======

/**
 * 返回拥有此 Action 的组件。
 */
UCPActionComponent* UCPAction::GetOwningComponent() const
{
	return ActionComp;
}

/**
 * 获取世界对象 World。
 * 注意：由于 UObject 没有直接的 World 引用，因此需要通过 Outer（通常是 Actor）间接获取。
 */
UWorld* UCPAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

// ====== 网络同步相关 ======

/**
 * bIsRunning 变量同步时调用。
 * 会在客户端上自动调用 Start 或 Stop 来还原状态。
 */
void UCPAction::OnRep_RepData()
{
	
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

/**
 * 设置需要网络同步的变量。
 * bIsRunning 和 ActionComp 都会在网络中同步。
 */
void UCPAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCPAction, RepData);
	DOREPLIFETIME(UCPAction, ActionComp);
}


