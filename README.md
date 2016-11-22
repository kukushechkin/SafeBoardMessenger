# SafeBoardMessenger
Библиотека взаимодействия с сервером Safe Board Messenger.

# Подключение библиотеки к win32 проекту
1. Добавить в свойства проекта в раздел Linker(Компоновщик) дополнительный путь к библиотекам: Components\messenger\lib\win\x86
2. Добавить в свойства проекта в раздел Linker(Компоновщик)->Input(Ввод) дополнительные библиотеки: libmessenger.lib, expat.lib, libstrophe.lib
3. Добавить в свойства проекта в раздел С++ дополнительный путь к заголовкам: Components\messenger\include

# Подключение библиотеки к iOS/OSX проекту
1. Переходим в настройки проекта и выбираем таргет к которому хотим подключить библиотеку
2. Переходим в закладку Build Phases и в разделе Link Binary With Libraries добавляем библиотеку libmessenger.a
3. Переходим в закладку Build Settings и в поле Library Search Paths дописываем путь до директории с библиотекой libmessenger.a (Components/messenger/lib/ios или Components/messenger/lib/osx)
4. В той же закладке в поле Header Search Paths дописываем путь до заголовочных файлов библиотеки libmessenger.a (Components/messenger/include)