// Ŀ�ǵ� ��ɾ�				�ɼ�				���� ������ �ִ� ��ġ			�纻 ������ ������ ��ġ

xcopy			/y				.\Engine\Header\*.*						.\Reference\Header\

xcopy			/y				.\Engine\System\Bin\System.dll			.\Client\Bin\
xcopy			/y				.\Engine\Utility\Bin\Utility.dll		.\Client\Bin\

xcopy			/y				.\Engine\System\Bin\System.lib			.\Reference\Library\
xcopy			/y				.\Engine\Utility\Bin\Utility.lib		.\Reference\Library\

xcopy			/d /e /y			.\Resource\Texture					.\Client\Bin\Resource\Texture\
xcopy			/d /e /y			.\Resource\Sound					.\Client\Bin\Resource\Sound\

xcopy			/d /e /y			.\Resource\FMOD						.\Client\Bin\