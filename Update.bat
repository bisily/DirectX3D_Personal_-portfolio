// 명령어					옵션			카피할 경로							카피 받을 경로

xcopy					/y			.\Engine\System\Bin\System.lib				.\Reference\Libraries\
xcopy					/y			.\Engine\System\Bin\System.dll				.\Client\Bin\
xcopy					/y			.\Engine\System\Bin\System.dll				.\MyMap_Tool\Bin\

xcopy					/y			.\Engine\Resources\Bin\Resources.lib		.\Reference\Libraries\
xcopy					/y			.\Engine\Resources\Bin\Resources.dll		.\Client\Bin\
xcopy					/y			.\Engine\Resources\Bin\Resources.dll		.\MyMap_Tool\Bin\

xcopy					/y			.\Engine\Utility\Bin\Utility.lib			.\Reference\Libraries\
xcopy					/y			.\Engine\Utility\Bin\Utility.dll			.\Client\Bin\
xcopy					/y			.\Engine\Utility\Bin\Utility.dll			.\MyMap_Tool\Bin\

xcopy					/y			.\Engine\System\Codes\*.h					.\Reference\Headers\
xcopy					/y			.\Engine\Resources\Codes\*.h				.\Reference\Headers\
xcopy					/y			.\Engine\Utility\Codes\*.h					.\Reference\Headers\
xcopy					/y			.\Engine\Headers\*.h						.\Reference\Headers\

xcopy					/y			.\Engine\Export\*.h							.\Reference\Headers\
xcopy					/y			.\Engine\Export\*.inl						.\Reference\Headers\




