// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System.Collections.Generic;

public class KBEnginePlugins : ModuleRules
{
    public KBEnginePlugins(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        bEnableUndefinedIdentifierWarnings = false;

        string[] PrivateModules = new string[] { "Slate", "SlateCore", "Networking", "Sockets", "OpenSSL" };
        string[] PublicModules = new string[] { "Core", "CoreUObject", "Engine"};
        List<string> PublicModulesList = new List<string>(PublicModules);

        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
        if (Target.bBuildEditor)
        {
            PublicModulesList.Add("UnrealEd");
        }

        PublicModules = PublicModulesList.ToArray();
        PublicDependencyModuleNames.AddRange(PublicModules);

        PrivateDependencyModuleNames.AddRange(PrivateModules);

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
        );
    }
}