#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Runtime/Engine/Public/GenericQuadTree.h"
#include "SimpleWFC.generated.h"

UENUM(BlueprintType)
enum class EProtoType : uint8
{
	VE_None,
	VE_0,
	VE_1,
	VE_2,
	VE_3,
	VE_4,
	VE_5,
	VE_6,
	VE_7,
	VE_8,
	VE_9,
	VE_10,
	VE_11,
	//VE_12
};

USTRUCT(BlueprintType)
struct FProto
{
	GENERATED_BODY()
public:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Proto")
	TArray<EProtoType> NeighborTypeList_PX;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Proto")
	TArray<EProtoType> NeighborTypeList_NX;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Proto")
	TArray<EProtoType> NeighborTypeList_PY;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Proto")
	TArray<EProtoType> NeighborTypeList_NY;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Proto")
	EProtoType ProtoType = EProtoType::VE_None;
	//Proto Assets, maybe a prefab or a mesh or other something
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proto")
	TSubclassOf<AActor> ProtoAsset = nullptr;
};

USTRUCT(BlueprintType)
struct FWFCCell
{
	GENERATED_BODY()
public:
	
	FWFCCell(){}

	bool bCollapse = false;
	//每次Propagation之前先清理掉所有的cell的标记,防止一个cell被多次计算
	bool bDirty = false;
	TArray<FProto*>PotentialProto;
	FIntVector CellListLocation = FIntVector(-1, -1, -1);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "SimpleWFC")
	FVector CellLocation = FVector(0, 0, 0);
};

UCLASS(BlueprintType)
class ASimpleWFC : public AActor
{
	GENERATED_BODY()
public:

	ASimpleWFC(const FObjectInitializer& ObjectInitializer);
	~ASimpleWFC();

	TQuadTree<uint32>* CellLIndexTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleWFC")
	int32 RandSeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleWFC")
	TArray<FWFCCell>CellList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleWFC")
	TArray<FProto>ProtoDataList;

	UFUNCTION(BlueprintCallable, Category = "SimpleWFC")
	void CollapseCellByIndex(int32 cellIndex);

	UFUNCTION(BlueprintCallable, Category = "SimpleWFC")
	bool Observation(const FWFCCell& LastPropagationCell, FWFCCell& CurrPropagationCell, const int32& DirectionIndex);

	UFUNCTION(BlueprintCallable, Category = "SimpleWFC")
	FProto GetCollapseProtoCopy(int32 index)
	{
		if (CellList[index].bCollapse)
			return *CellList[index].PotentialProto[0];
		else
			return FProto();
	}

	void Propagation();

	//空间大小，单位是米
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleWFC")
	int32 CellSize = 5;
	//空间大小，单位是米
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleWFC")
	int32 ObjectBoundBoxSize = 210;
	int32 ObjectSize;

private:
	TArray<FWFCCell*> LastPropagationCellsArray;
	TArray<FWFCCell*> CurrPropagationCellsArray;

	FRandomStream SimpleWFCCreateStream;
};