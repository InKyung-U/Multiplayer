// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);
	}
	else
		return false;

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);
	}
	else
		return false;

	return true;
}

void UInGameMenu::CancelPressed()
{
	Teardown();
}

void UInGameMenu::QuitPressed()
{
	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadMainMenu();
	}
}
