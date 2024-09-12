#include "GrassControl/Events.h"

cellLoadEventHandler* cellLoadEventHandler::GetSingleton()
{
	static cellLoadEventHandler singleton;
	return &singleton;
}

MenuOpenCloseEventHandler* MenuOpenCloseEventHandler::GetSingleton()
{
	static MenuOpenCloseEventHandler singleton;
	return &singleton;
}

void MenuOpenCloseEventHandler::Register()
{
	RE::UI::GetSingleton()->AddEventSink(GetSingleton());
}

void cellLoadEventHandler::Register()
{
	RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(GetSingleton());
}

RE::BSEventNotifyControl cellLoadEventHandler::ProcessEvent(const RE::TESCellAttachDetachEvent* a_event, RE::BSTEventSource<RE::TESCellAttachDetachEvent>*)
{
	if (a_event) {
		auto refr = a_event->reference;
		if (!refr)
			return RE::BSEventNotifyControl::kContinue;
		auto cell = refr->GetParentCell();
		if (!cell)
			return RE::BSEventNotifyControl::kContinue;
		if (cell->IsInteriorCell()) {
			if (GrassControl::Config::OnlyLoadFromCache && GrassControl::Config::ExtendGrassDistance) {
				GrassControl::DistantGrass::LO2Map->UnloadAll();
			}
		}
	}

	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl MenuOpenCloseEventHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
{
	if (a_event) {
#ifndef NDEBUG
		if (a_event->menuName == RE::Console::MENU_NAME) {
#else
		if (a_event->menuName == RE::MainMenu::MENU_NAME) {
#endif
			if (a_event->opening) {
				GrassControl::GrassControlPlugin::OnMainMenuOpen();
			}
		}
	}

	return RE::BSEventNotifyControl::kContinue;
}
