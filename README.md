<h1>Структура проекта</h1>

 <h3>databases — каталог с файлом бд SQLite</h3>
 <h3>main.py — точка входа</h3>
 <h3>features/routers.py — HTTP‑роутеры</h3>
 <h3>features/boards.py — сервисный слой для досок</h3>
 <h3>features/tasks.py — сервисный слой для задач</h3>
 <h3>features/ORM_models.py — ORM‑модели</h3>
 <h3>features/schemas.py — Pydantic v2‑схемы с типами и валидаторами</h3>
 <h3>features/database.py — слой работы с базой данных</h3>
<h1>QuickStart</h1>
 <h3>python -m uvicorn main:app --host 0.0.0.0 --port 8000</h3>
 <h3>Либо через докер</h3>

