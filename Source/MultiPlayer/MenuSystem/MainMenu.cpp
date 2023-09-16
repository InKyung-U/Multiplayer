// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));

	if (ServerRowBPClass.Class == nullptr)
		return;
	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpeHostMenu);
	}
	else
		return false;

	if (CancelHostMenuButton)
	{
		CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	}
	else
		return false;

	if (ConfirmHostMenuButton)
	{
		ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	}
	else
		return false;

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	}
	else
		return false;

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	}
	else
		return false;

	if (ConfirmJoinMenuButton)
	{
		ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	}
	else
		return false;

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);
	}
	else
		return false;
	

	return true;
}

void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		FString ServerName = ServerHostName->Text.ToString();
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))
		return;

	ServerList->ClearChildren();
	uint32 i = 0;
	for(const auto& ServerData : ServerNames)
	{
		auto Row = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(Row != nullptr))
			return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUsername));
		Row->ConnectionFraction->SetText(
			FText::FromString(FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers)));

		Row->Setup(this, i);
		++i;
		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectesIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));

		if (Row)
		{
			Row->Selected = ((SelectesIndex.IsSet()) && (SelectesIndex.GetValue() == i));
		}
	}
}

void UMainMenu::JoinServer()
{
	if (SelectesIndex.IsSet() && MenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index %d"), SelectesIndex.GetValue());
		MenuInterface->Join(SelectesIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index not set."));
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	if (!ensure(JoinMenu != nullptr))
		return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshSeverList();
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	if (!ensure(MainMenu != nullptr))
		return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
		return;

	PlayerController->ConsoleCommand("quit");
}

void UMainMenu::OpeHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	if (!ensure(HostMenu != nullptr))
		return;
	MenuSwitcher->SetActiveWidget(HostMenu);
	
}
