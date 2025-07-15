#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CPAction.generated.h"

class UCPActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
	
};

/**
 * 基础动作类，可被添加到 ActionComponent 以组成技能系统
 */
UCLASS(Blueprintable)
class CPP_PRACTICE_API UCPAction : public UObject
{
	GENERATED_BODY()

public:

	// ======== Public Properties ========
	
	/** 自动启动（默认在 BeginPlay 立刻激活） */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	/** 行为名字 */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	// ======== Public Methods ========

	/** 初始化绑定组件 */
	void Initialize(UCPActionComponent* NewActionComp);

	/** 是否正在运行 */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	/** 能否开始运行 */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	/** 开始运行 */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	/** 停止运行 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	/** 网络支持声明 */
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	/** 提供 GetWorld 支持 */
	virtual UWorld* GetWorld() const override;

protected:

	// ======== Protected Properties ========

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;
	
	/** 动作所依附的组件 */
	UPROPERTY(Replicated)
	UCPActionComponent* ActionComp;

	/** 当前是否正在运行，RepNotify */
	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;
	//bool bIsRunning;

	/** 启用时授予的标签 */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/** 被这些标签阻挡，不能启动 */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(Replicated)
	float TimeStarted;
	
	// ======== Protected Methods ========

	/** 本地调用：执行运行状态的视觉更新等 */
	UFUNCTION()
	void OnRep_RepData();

	/** 获取拥有者组件 */
	UFUNCTION(BlueprintCallable, Category = "Action")
	UCPActionComponent* GetOwningComponent() const;
};
