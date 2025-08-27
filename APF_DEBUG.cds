<distribution version="13.0.0" name="APF_DEBUG" type="MSI">
	<prebuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></prebuild>
	<postbuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></postbuild>
	<msi GUID="{EC0BB04C-79F3-4870-8594-29F81A3342F3}">
		<general appName="APF_DEBUG" outputLocation="e:\work\APF\uAPF_Study\firmware\20140114\software\后台(第二版硬件)20140605\cvidistkit.APF_DEBUG" relOutputLocation="cvidistkit.APF_DEBUG" outputLocationWithVars="e:\work\APF\uAPF_Study\firmware\20140114\software\后台(第二版硬件)20140605\cvidistkit.%name" relOutputLocationWithVars="cvidistkit.%name" upgradeBehavior="1" autoIncrement="true" version="1.0.13">
			<arp company="%company" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="Chinese (Simplified)" showPaths="true" readMe="" license="">
			<dlgstrings welcomeTitle="APF_DEBUG" welcomeText=""/></userinterface>
		<dirs appDirID="100">
			<installDir name="[Program Files]" dirID="2" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="APF_DEBUG" dirID="100" parentID="2" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="[Start&gt;&gt;Programs]" dirID="7" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="APF_DEBUG" dirID="101" parentID="7" isMSIDir="false" visible="true" unlock="false"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="e:\work\APF\uAPF_Study\firmware\20140114\software\后台(第二版硬件)20140605\cvibuild.APF_DEBUG\Release\APF_DEBUG.exe" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="e:\work\APF\uAPF_Study\firmware\20140114\software\后台(第二版硬件)20140605\APF_DEBUG.uir" relSourcePath="APF_DEBUG.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<fileGroups>
			<projectOutput targetType="0" dirID="100" projectID="0">
				<fileID>0</fileID></projectOutput>
			<projectDependencies dirID="100" projectID="0"/></fileGroups>
		<shortcuts>
			<shortcut name="APF_DEBUG" targetFileID="0" destDirID="101" cmdLineArgs="" description="" runStyle="NORMAL"/></shortcuts>
		<mergemodules/>
		<products>
			<product name="NI LabWindows/CVI Shared Run-Time Engine" UC="{80D3D303-75B9-4607-9312-E5FC68E5BFD2}" productID="{A1D6B35A-7ECC-4E12-9413-D47E81609F0A}" path="C:\ProgramData\National Instruments\MDF\ProductCache\" flavorID="_full_" flavorName="Full" verRestr="false" coreVer="">
				<cutSoftDeps/></product></products>
		<runtimeEngine installToAppDir="false" activeXsup="true" analysis="true" cvirte="true" dotnetsup="true" instrsup="true" lowlevelsup="true" lvrt="true" netvarsup="true" rtutilsup="true">
			<hasSoftDeps/></runtimeEngine><sxsRuntimeEngine>
			<selected>false</selected>
			<doNotAutoSelect>false</doNotAutoSelect></sxsRuntimeEngine>
		<advanced mediaSize="650">
			<launchConditions>
				<condition>MINOS_WINXP</condition>
			</launchConditions>
			<includeConfigProducts>true</includeConfigProducts>
			<maxImportVisible>silent</maxImportVisible>
			<maxImportMode>merge</maxImportMode>
			<custMsgFlag>false</custMsgFlag>
			<custMsgPath>c:\program files (x86)\national instruments\cvi2010\bin\msgrte.txt</custMsgPath>
			<signExe>false</signExe>
			<certificate></certificate>
			<signTimeURL></signTimeURL>
			<signDescURL></signDescURL></advanced>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="e:\work\APF\uAPF_Study\firmware\20140114\software\后台(第二版硬件)20140605\APF_DEBUG.prj" ProjectRelativePath="APF_DEBUG.prj"/></Projects>
		<buildData progressBarRate="1.443076974094843">
			<progressTimes>
				<Begin>0.000000000000000</Begin>
				<ProductsAdded>0.078145160034179</ProductsAdded>
				<DPConfigured>1.555576777099609</DPConfigured>
				<DPMergeModulesAdded>2.198087544799805</DPMergeModulesAdded>
				<DPClosed>4.406344979248047</DPClosed>
				<DistributionsCopied>4.525185048339844</DistributionsCopied>
				<End>69.296372816650390</End></progressTimes></buildData>
	</msi>
</distribution>
