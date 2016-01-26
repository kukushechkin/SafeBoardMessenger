# SafeBoardMessenger
Библиотека взаимодействия с сервером Safe Board Messenger.

# Подключение библиотеки к win32 проекту
1. Добавить в свойства проекта в раздел Linker(Компоновщик) дополнительный путь к библиотекам: Components\messenger\lib\win\x86
2. Добавить в свойства проекта в раздел Linker(Компоновщик)->Input(Ввод) дополнительные библиотеки: libmessenger.lib, expat.lib, libstrophe.lib
3. Добавить в свойства проекта в раздел С++ дополнительный путь к заголовкам: Components\messenger\include