# <center>ximan</center>

  

## Описание



ximan - Пакетный менеджер для операционной системы [Govnos](https://github.com/xi816/GovnOS) 


## Установка

```bash
chmod +x setup.sh req.sh && ./req.sh && sudo ./setup.sh
```

## Как использовать

#### Обновление списка пакетов:
```bash
ximan update
```

#### Установка пакетов:
```bash
ximan install <package_name>
```
Пакет установится в  /usr/share/ximan/<package_name>/

#### Установка пакетов в home:
```bash
ximan install -h <package_name>
```
Пакет установится в /home/user/<package_name>/

#### Обновление пакета:
```bash
ximan upgrade <package_name>
```
Обновится пакет в /usr/share/ximan/<package_name>/

#### Обновление пакета в home:
```bash
ximan upgrade -h <package_name>
```
Обновится пакет в /home/user/<package_name>/



## Тип лицензии

  

Этот проект распространяется под лицензией [GPL](LICENSE).