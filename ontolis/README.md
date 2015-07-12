ontolis
=======

Ontology editor

Как посмотреть пример работы
-------------------------
* Настроить проект Qt так, чтобы он собирал в папку рядом с исходными кодами, но без указания версии. Например, рядом с папкой `ontolis` должна оказаться папка `build-ontolis-Debug`, с `ontolis-avis-node-roundedrect` - `build-ontolis-avis-node-roundedrect-Debug` и т.д.
* Собрать `ontolis-lib`, `ontolis-avis-node-roundedrect`, `ontolis-avis-node-ellipse`, `ontolis`
* Запустить `ontolis`
* File -> Open ontology... -> ontolis/examples/_visualizeMaster.ont -> Open
* Graphics -> Apply

Теперь в Graphics -> Modify -> Nodes дописались поддерживаемаые визуализаторы, найденные в онтологии, которые можно править (имя поменяли - в онтологии вершина переименовалась)

TODO
-------------------------
* Возможность добавить новый визуализатор через Graphics -> Modify -> Nodes -> Add
* В редакторе визуализатора кнопка `Обзор` не настроена на открытие openfiledialog (или как он там в Qt)
* В редакторе визуализатора изменение параметров не синхронизируется с онтологией