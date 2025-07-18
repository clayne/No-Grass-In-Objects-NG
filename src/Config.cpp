#include <GrassControl/config.h>
#include <SimpleIni.h>

namespace GrassControl
{
	void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting)
	{
		const char* bFound = nullptr;
		bFound = a_ini.GetValue(a_sectionName, a_settingName);
		if (bFound) {
			a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
		}
	}

	void ReadIntSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, int& a_setting)
	{
		const char* bFound = nullptr;
		bFound = a_ini.GetValue(a_sectionName, a_settingName);
		if (bFound) {
			a_setting = static_cast<int>(a_ini.GetLongValue(a_sectionName, a_settingName));
		}
	}

	void ReadDoubleSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, double& a_setting)
	{
		const char* bFound = nullptr;
		bFound = a_ini.GetValue(a_sectionName, a_settingName);
		if (bFound) {
			a_setting = a_ini.GetDoubleValue(a_sectionName, a_settingName);
		}
	}

	void ReadStringSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName,
		std::string& a_setting)
	{
		const char* bFound = nullptr;
		bFound = a_ini.GetValue(a_sectionName, a_settingName);
		if (bFound) {
			a_setting = Util::StringHelpers::trim(a_ini.GetValue(a_sectionName, a_settingName));
		}
	}

	void Config::ReadSettings()
	{
		const auto readIni = [&](auto a_path) {
			CSimpleIniA ini;
			ini.SetUnicode();
			ini.SetMultiLine();

			if (ini.LoadFile(a_path.data()) >= 0) {
				// Debug
				ReadBoolSetting(ini, "Debug", "Debug-Log-Enable", DebugLogEnable);

				//Raycast
				ReadBoolSetting(ini, "RayCastConfig", "Ray-cast-enabled", RayCast);
				ReadDoubleSetting(ini, "RayCastConfig", "Ray-cast-height", RayCastHeight);
				ReadDoubleSetting(ini, "RayCastConfig", "Ray-cast-depth", RayCastDepth);
				ReadStringSetting(ini, "RayCastConfig", "Ray-cast-collision-layers", RayCastCollisionLayers);
				ReadStringSetting(ini, "RayCastConfig", "Ray-cast-ignore-forms", RayCastIgnoreForms);
				ReadStringSetting(ini, "RayCastConfig", "Ray-cast-texture-forms", RayCastTextureForms);
				ReadDoubleSetting(ini, "RayCastConfig", "Ray-cast-texture-width", RayCastTextureWidth);
				ReadBoolSetting(ini, "RayCastConfig", "Grass-cliffs-enabled", GrassCliffs);
				ReadStringSetting(ini, "RayCastConfig", "Grass-cliffs-forms", GrassCliffsForms);
				ReadIntSetting(ini, "RayCastConfig", "Ray-cast-mode", EnsureMaxGrassTypesPerTextureSetting);
				ReadDoubleSetting(ini, "RayCastConfig", "Ray-cast-width", RayCastWidth);

				//GrassConfig
				ReadBoolSetting(ini, "GrassConfig", "Super-dense-grass", SuperDenseGrass);
				ReadIntSetting(ini, "GrassConfig", "Super-dense-mode", SuperDenseMode);
				ReadBoolSetting(ini, "GrassConfig", "Profiler-report", ProfilerReport);
				ReadBoolSetting(ini, "GrassConfig", "Use-grass-cache", UseGrassCache);
				ReadBoolSetting(ini, "GrassConfig", "Extend-grass-distance", ExtendGrassDistance);
				ReadBoolSetting(ini, "GrassConfig", "Extend-grass-count", ExtendGrassCount);
				ReadIntSetting(ini, "GrassConfig", "Ensure-max-grass-types-setting", EnsureMaxGrassTypesPerTextureSetting);
				ReadDoubleSetting(ini, "GrassConfig", "Overwrite-grass-distance", OverwriteGrassDistance);
				ReadDoubleSetting(ini, "GrassConfig", "Overwrite-grass-fade-range", OverwriteGrassFadeRange);
				ReadIntSetting(ini, "GrassConfig", "Overwrite-min-grass-size", OverwriteMinGrassSize);
				ReadDoubleSetting(ini, "GrassConfig", "Global-grass-scale", GlobalGrassScale);
				ReadBoolSetting(ini, "GrassConfig", "Only-load-from-cache", OnlyLoadFromCache);
				ReadStringSetting(ini, "GrassConfig", "Skip-pregenerate-world-spaces", SkipPregenerateWorldSpaces);
				ReadStringSetting(ini, "GrassConfig", "Only-pregenerate-world-spaces", OnlyPregenerateWorldSpaces);
				ReadIntSetting(ini, "GrassConfig", "DynDOLOD-Grass-Mode", DynDOLODGrassMode);
				ReadIntSetting(ini, "GrassConfig", "Max-Failures", MaxFailures);
				ReadBoolSetting(ini, "GrassConfig", "Freeze-check-enabled", FreezeCheck);
				ReadBoolSetting(ini, "GrassConfig", "Updating-Cache", Updating);

				return true;
			}
			return false;
		};

		logger::info("Reading .ini...");
		if (readIni(iniPath)) {
			logger::info("...success");
		} else {
			logger::info("...ini not found, creating a new one");
			WriteSettings();
		}
	}

	void Config::WriteSettings()
	{
		logger::info("Writing .ini...");

		CSimpleIniA ini;
		ini.SetUnicode();

		ini.LoadFile(iniPath.data());

		// Debug
		ini.SetBoolValue("Debug", "Debug-Log-Enable", DebugLogEnable, ";Enable additional debug logging.");

		//Raycast
		ini.SetBoolValue("RayCastConfig", "Ray-cast-enabled", RayCast, ";Use ray casting to detect where grass shouldn't grow (inside rocks or roads).");
		ini.SetDoubleValue("RayCastConfig", "Ray-cast-height", RayCastHeight, ";The distance above grass that must be free. 200 is slightly more than player height.");
		ini.SetDoubleValue("RayCastConfig", "Ray-cast-depth", RayCastDepth, ";The distance below grass that must be free.");
		ini.SetValue("RayCastConfig", "Ray-cast-collision-layers", RayCastCollisionLayers.c_str(), ";Which collision layers to check when raycasting. Not recommended to change unless you know what you're doing. These are collision layer index from CK separated by space.");
		ini.SetValue("RayCastConfig", "Ray-cast-ignore-forms", RayCastIgnoreForms.c_str(), ";Which objects will raycast ignore. This can be useful if you want grass to still grow from some objects (such as roads).The format is formid : formfile separated by; for example \"1812A:Skyrim.esm;1812D:Skyrim.esm\" would add RoadChunkL01 and RoadChunkL02 forms to ignore list. Base forms go here not object references !");
		ini.SetValue("RayCastConfig", "Ray-cast-texture-forms", RayCastTextureForms.c_str(), ";Which landcape textures raycast will check for. This can be useful if you want to prevent grass from growing on certain landscape textures (such as roads textures from Northern Roads). The format is formid : formfile separated by; for example \"425FE:Northern Roads.esp;2D5520:Northern Roads.esp\" would add COTN_LRoadStone01 and COTN_LRoadDirt01Trail77 forms to the texture list to check. Lanscape textures go here not texturesets !");
		ini.SetDoubleValue("RayCastConfig", "Ray-cast-texture-width", RayCastTextureWidth, ";The distance from the grass in each direction that should be checked as well as the point under the grass for the land texture(s) specified in Ray-cast-texture-forms.");
		ini.SetBoolValue("RayCastConfig", "Grass-cliffs-enabled", GrassCliffs, ";Use ray casting to detect dirt cliffs and place grass on them");
		ini.SetValue("RayCastConfig", "Grass-cliffs-forms", GrassCliffsForms.c_str(), ";The forms of the dirt cliffs in the game that will be converted to grass cliffs.");
		ini.SetLongValue("RayCastConfig", "Ray-cast-mode", RayCastMode, ";Switch how ray casting will work. Valid values: 0 = original simple ray-cast that check for collision above and below using a line; 1 = new shape based ray-cast method that uses a cylinder shape with a specified width or by default a width based upon the grass bounds. This is functionally very different from the original line method and will perform differently ; 2 = same as 1 with a box with by default the same dimensions as the grass bounds or a square that uses the specified width for width and length.");
		ini.SetDoubleValue("RayCastConfig", "Ray-cast-width", RayCastWidth, ";The distance from the center or width of the shape used in ray cast modes 1 and 2. If this value is a 0, then a width will be calculated from the grass bounds for each grass type.");

		//GrassConfig
		ini.SetBoolValue("GrassConfig", "Super-dense-grass", SuperDenseGrass, ";Enable much more grass without having to change mod files.");
		ini.SetLongValue("GrassConfig", "Super-dense-mode", SuperDenseMode, ";How the super dense mode is achieved. Not recommended to change for normal play. This does nothing unless you enable SuperDenseGrass setting.7 is normal game(meaning nothing is actually changed), 6 or less would be much less grass, 8 is dense, 9 is ?, 10 + is probably crash city.");
		ini.SetBoolValue("GrassConfig", "Profiler-report", ProfilerReport, ";Whether to track how much time is taken to generate grass. Whenever you open console the result is reported. Try disabling all settings except profiler, go to game and in main menu 'coc riverwood', after loading open console to see normal game time. Then enable settings and check again how it changed. Remember to coc from main menu instead of loading save because it might not be accurate otherwise.");
		ini.SetBoolValue("GrassConfig", "Use-grass-cache", UseGrassCache, ";This will generate cache files in /Data/Grass/ and use those when we have them. Any time you change anything with your mod setup you must delete the contents of that directory so new cache can be generated or you will have bugs like floating grass or still grass in objects(that were changed by mods).");
		ini.SetBoolValue("GrassConfig", "Extend-grass-distance", ExtendGrassDistance, ";Set true if you want to enable extended grass distance mode. This will allow grass to appear outside of loaded cells.Relevant ini settings : SkyrimPrefs.ini[Grass] fGrassStartFadeDistance");
		ini.SetBoolValue("GrassConfig", "Extend-grass-count", ExtendGrassCount, ";Allow more grass to be made in total. This is needed if you use very dense grass or have large draw distance. Otherwise it will reach a limit and just stop making grass leaving weird empty squares.");
		ini.SetLongValue("GrassConfig", "Ensure-max-grass-types-setting", EnsureMaxGrassTypesPerTextureSetting, ";Makes sure that the max grass types per texture setting is set to at least this much. Can be useful to make sure your INI isn't being overwritten by anything. Most grass replacer mods require this. Set 0 to disable any verification.");
		ini.SetDoubleValue("GrassConfig", "Overwrite-grass-distance", OverwriteGrassDistance, ";Overwrite fGrassStartFadeDistance from any INI. If this is zero or higher then the grass distance will always be taken from this configuration instead of any INI.This can be useful if you have a million things overwriting your INI files and don't know what to edit, so you can just set it here. For example 7000 is vanilla highest in-game grass slider. If you want to set higher you need to enable ExtendGrassDistance setting as well or it will not look right in - game. What the setting actually means is that grass will start to fade out at this distance. Actual total grass distance is this value + fade range value.");
		ini.SetDoubleValue("GrassConfig", "Overwrite-grass-fade-range", OverwriteGrassFadeRange, ";Overwrite fGrassFadeRange from any INI. If this is zero or higher then the grass fade range will always be taken from this configuration instead of any INI. This determines the distance it takes for grass to completely fade out starting from OverwriteGrassDistance(or fGrassStartFadeDistance if you didn't use the overwrite). If you want the grass fade out to not be so sudden then increase this value.Probably recommended to keep this at least half of the other setting.");
		ini.SetLongValue("GrassConfig", "Overwrite-min-grass-size", OverwriteMinGrassSize, ";Overwrite iMinGrassSize from any INI. If this is zero or higher then the grass density setting (iMinGrassSize) will be taken from here instead of INI files. Lower values means more dense grass. 50 or 60 is normal mode with somewhat sparse grass(good performance).Lowering the value increases density !20 is very high density. You should probably not set lower than 20 if you decide to change it. The grass form density setting itself still mostly controls the actual density of grass, think of iMinGrassSize more of as a cap for that setting.");
		ini.SetDoubleValue("GrassConfig", "Global-grass-scale", GlobalGrassScale, ";Apply this scale to every piece of grass everywhere. For example 0.5 makes all grass pieces half the size it should be.");
		ini.SetBoolValue("GrassConfig", "Only-load-from-cache", OnlyLoadFromCache, ";This will change how grass works. It means grass can only ever be loaded from files and not generated during play.");
		ini.SetValue("GrassConfig", "Skip-pregenerate-world-spaces", SkipPregenerateWorldSpaces.c_str(), ";When pre-generating grass then skip worldspaces with this editor IDs. This can greatly speed up generating if we know the worldspace will not need grass at all.");
		ini.SetValue("GrassConfig", "Only-pregenerate-world-spaces", OnlyPregenerateWorldSpaces.c_str(), ";If this is not empty then skip every worldspace that isn't in this list.");
		ini.SetLongValue("GrassConfig", "DynDOLOD-Grass-Mode", DynDOLODGrassMode, ";Enable grass compatibility mode with DynDOLOD Grass LODs. Valid values: 0 = Disabled; 1 = Display grass only in active cells(without fade) and let DynDOLOD handle inactive cell Grass; 2 = Display grass only in active cells and large ref loaded cells(without fade) and let DynDOLOD handle grass outside of large ref cells ");
		ini.SetLongValue("GrassConfig", "Max-Failures", MaxFailures, ";Maximum number of attempts to generate a grass cache before skipping a cell.");
		ini.SetBoolValue("GrassConfig", "Freeze-check-enabled", FreezeCheck, ";Enable or disable the freeze check that is run while generating a grass cache. Recommended to leave enabled unless the message repeately appears.");
		ini.SetBoolValue("GrassConfig", "Updating-Cache", Updating, ";If you are generating a cache for a worldspace you have not generated yet, set this true. Otherwise, the cache files in your grass folder will be deleted. If you are updating a worldspace, then move all files you do not want to be deleted to a new folder and replace the old files in the folder once you have generated new cache files. Then rename/move the folder back.");

		ini.SaveFile(iniPath.data());

		logger::info("...success");
	}
}
