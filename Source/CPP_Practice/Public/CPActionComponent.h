// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CPActionComponent.generated.h"

class UCPAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPP_PRACTICE_API UCPActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// 构造函数
	UCPActionComponent();

	// Blueprint 调用的 Action 系统接口
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(AActor* Instigator, TSubclassOf<UCPAction> ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveAction(UCPAction* ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool HasAction(TSubclassOf<UCPAction> ActionClass) const;

	// 公共属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	// 网络复制接口
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	// Tick
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	// BeginPlay 生命周期函数
	virtual void BeginPlay() override;

	// 服务端启动 Action 的 RPC
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	// 服务端停止 Action 的 RPC
	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

	// 初始默认 Action 类列表
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<TSubclassOf<UCPAction>> DefaultActions;

	// 当前运行中的 Action 实例（由服务器同步）
	UPROPERTY(Replicated)
	TArray<UCPAction*> Actions;
};