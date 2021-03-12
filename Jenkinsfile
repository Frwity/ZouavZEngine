pipeline
{
    agent any
	parameters
    {
        choice(name: 'Configuration', choices: ['Release','Debug'], description: 'Client Configuration')
		choice(name: 'Platform', choices: ['x64', 'Win32'], description: 'Platform architecture')
    }
	environment
	{
		configFilePath = "Config.json"
		MSBuildPath = "\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe\""
		
		def configFile = readJSON file: "${env.configFilePath}"
		Name = "${configFile.Project.Name}"
	}
	stages
	{
	    stage('checkout')
	    {
	        steps
	        {
				cleanWs()
	            script
	            {
	                if( currentBuild.rawBuild.getCause(hudson.model.Cause$UserIdCause) != null )
                        checkout([$class: 'GitSCM', branches: [[name: Branch[7..-1]]], doGenerateSubmoduleConfigurations: false, extensions: [], submoduleCfg: [], userRemoteConfigs: [[credentialsId: 'e.kraemer_git_SSH', url: 'ssh://git@git.isartintra.com:2424/2020/GP_2023_Engine/Groupe_3.git']]])
                    else 
                        checkout([$class: 'GitSCM', branches: [[name: '**']], doGenerateSubmoduleConfigurations: false, extensions: [], submoduleCfg: [], userRemoteConfigs: [[credentialsId: 'e.kraemer_git_SSH', url: 'ssh://git@git.isartintra.com:2424/2020/GP_2023_Engine/Groupe_3.git']]])
	            }
            }
	    }
		stage('Version')
		{
			steps
			{
				script
				{
					if(env.BRANCH.contains("release"))
					{
						//String a = "release-04.05.06";
						def versions = (a =~ /\d+/).findAll();						
						env.VERSION = versions.join('.');
						
						def configFile = readJSON file: "${env.configFilePath}"
						configFile.Project.Version = versions as String
						writeJSON(file: "${env.configFilePath}", json: configFile, pretty: 4)
					}
					else
					{
						echo "Update version number file"
						
						def configFile = readJSON file: "${env.configFilePath}"
						env.VERSION = (configFile.Project.Version as String[]).join('.');
					}
					echo env.VERSION
				}
			}			
		}
		stage("Build")
		{
			steps
			{
				bat "${env.MSBuildPath} \"${WORKSPACE}\\ZouavZEngine.sln\" /t:ZouavZEngine /p:Configuration=${params.Configuration} /p:Platform=${params.Platform} "
			}
			post
    		{
    			success
    			{
    			    script
    			    {
    			        def configFile = readJSON file: "${env.configFilePath}"
    					for(String fileName in configFile.Project.Dependencies) 
    					{
    					    
    						def file = findFiles(glob: "**/${fileName}")
    						echo "${file}"
    						bat("copy /Y \"${file[0].path}\" \"${WORKSPACE}\\${params.Platform}\\${params.Configuration}\"")
    					}
    					for(String dirName in configFile.Project.ResourcesDir) 
    					{
    						echo "${dirName}"
    						bat("move /Y ${dirName} \"${WORKSPACE}\\${params.Platform}\\${params.Configuration}\"")
    					}
						echo "Zip contains"
						bat "dir \"${WORKSPACE}\\${params.Platform}\\${params.Configuration}\""
						
    					bat "dir"
    					def exists = fileExists "${env.Name}_${env.VERSION}.zip"
    					if(exists)
    					    bat "del \"${env.Name}_${env.VERSION}.zip\""
    					
    					zip archive: true, dir: "${WORKSPACE}\\${params.Platform}\\${params.Configuration}", zipFile: "${env.Name}_${env.VERSION}.zip"
						archiveArtifacts artifacts: "${env.Name}_${env.VERSION}.zip"
    			    }
    			}
    		}
		}
		
		stage('Documentation')
		{
	    	when 
    		{
				anyOf
				{
					expression
					{
						return env.BRANCH.contains("release")
					}
					expression
					{
						return env.BRANCH.contains("master")
					}
    			}
    		}
		    stages
		    {
        		stage('MarkDown')
            	{
                    steps
                    {
                        script
                        {
                            def configFile = readJSON file: "${env.configFilePath}"
                            for (def file in configFile.Project.MarkDown) 
                            {
            					echo "${file}"
            					pwsh """\$md = ConvertFrom-Markdown -Path \"${file.File}\"
            					\$md.Html | Out-File -Encoding ${file.Encoding} ./${file.HTMLName}"""
            					
            					archiveArtifacts artifacts: "${file.HTMLName}", onlyIfSuccessful: true
            				}
                        }
            	    }
            	}
            	stage('Doxygen')
            	{
            	    steps
            	    {
            	        bat "doxygen Doxyfile"
						zip archive: true, dir: "${WORKSPACE}\\html", zipFile: "DoxygenHTML.zip"
						archiveArtifacts artifacts: "DoxygenHTML.zip", onlyIfSuccessful: true
            	    }
            	}
		    }
		}
	}
}