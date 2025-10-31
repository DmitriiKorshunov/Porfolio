#!/usr/bin/env python3.10
# -*- coding: utf-8 -*-


from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.orm import Session
from features.database import SessionLocal
from features import boards as boards_service
from features import tasks as tasks_service
from features import schemas


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


routers_boards = APIRouter(prefix="/boards", tags=["Доски"])
routers_tasks = APIRouter(prefix="/tasks", tags=["Задачи"])


# ----------РУЧКИ ДЛЯ ДОСОК ----------

@routers_boards.get(
    "",
    response_model=list[schemas.BoardOut],
    operation_id="boards_list",
    summary="Получить список всех досок"
)
def list_boards_endpoint(db: Session = Depends(get_db)):
    return boards_service.list_boards(db)


@routers_boards.get(
    "/{board_id}",
    response_model=schemas.BoardOut,
    operation_id="boards_get_by_id",
    summary="Получить доску по id"
)
def get_board_endpoint(board_id: int, db: Session = Depends(get_db)):
    board = boards_service.get_board(db, board_id)
    if not board:
        raise HTTPException(status_code=404, detail="Доска не найдена")
    return board


@routers_boards.post(
    "",
    response_model=schemas.BoardOut,
    status_code=status.HTTP_201_CREATED,
    operation_id="boards_create",
    summary="Cоздать новую доску"
)
def create_board_endpoint(payload: schemas.BoardCreate, db: Session = Depends(get_db)):
    return boards_service.create_board(db, payload)


@routers_boards.put(
    "/{board_id}",
    response_model=schemas.BoardOut,
    operation_id="boards_put_update",
    summary="Полностью обновить доску",
    responses={
        422: {
            "description": "Ошибка валидации данных",
            "content": {"application/json": {"example": {
                "error": "Некорректные данные запроса",
                "detail": [{"loc": ["body", "name"], "msg": "field required", "type": "value_error.missing"}]
            }}}
        }
    })
def put_board_endpoint(board_id: int, payload: schemas.BoardCreate, db: Session = Depends(get_db)):
    board = boards_service.get_board(db, board_id)
    if not board:
        raise HTTPException(status_code=404, detail="Доска не найдена")
    return boards_service.update_board(db, board, payload)


@routers_boards.patch(
    "/{board_id}",
    response_model=schemas.BoardOut,
    operation_id="boards_patch_update",
    summary="Частично обновить доску"
)
def patch_board_endpoint(board_id: int, payload: schemas.BoardUpdate, db: Session = Depends(get_db)):
    board = boards_service.get_board(db, board_id)
    if not board:
        raise HTTPException(status_code=404, detail="Доска не найдена")
    return boards_service.update_board(db, board, payload)


@routers_boards.delete(
    "/boards/{board_id}",
    status_code=status.HTTP_204_NO_CONTENT,
    operation_id="boards_delete",
    summary="Удалить доску",
    responses={
        204: {
            "description": "Успешно удалена"
        }})
def delete_board_endpoint(board_id: int, db: Session = Depends(get_db)):
    board = boards_service.get_board(db, board_id)
    if not board:
        raise HTTPException(status_code=404, detail="Доска не найдена")
    boards_service.delete_board(db, board)


# ----------РУЧКИ ДЛЯ ЗАДАЧ ----------

@routers_boards.get(
    "/{board_id}/tasks",
    response_model=list[schemas.TaskOut],
    operation_id="tasks_list_for_board",
    summary="Получить задачи на указанной доске"
)
def list_board_tasks_endpoint(board_id: int, db: Session = Depends(get_db)):
    return tasks_service.list_tasks_for_board(db, board_id)


@routers_tasks.get(
    "/{task_id}",
    response_model=schemas.TaskOut,
    operation_id="tasks_get_by_id",
    summary="Получить задачу по id"
)
def get_task_endpoint(task_id: int, db: Session = Depends(get_db)):
    task = tasks_service.get_task(db, task_id)
    if not task:
        raise HTTPException(status_code=404, detail="Task not found")
    return task


@routers_boards.post(
    "/{board_id}/tasks",
    response_model=schemas.TaskOut,
    status_code=status.HTTP_201_CREATED,
    operation_id="tasks_create_for_board",
    summary="Создать задачу на доске"
)
def create_task_endpoint(board_id: int, payload: schemas.TaskCreate, db: Session = Depends(get_db)):
    try:
        return tasks_service.create_task(db, board_id, payload)
    except tasks_service.BoardNotFound:
        raise HTTPException(status_code=404, detail="Доска не найдена")


@routers_tasks.put(
    "/{task_id}",
    response_model=schemas.TaskOut,
    operation_id="tasks_put_update",
    summary="Полностью обновить задачу"
)
def put_task_endpoint(task_id: int, payload: schemas.TaskCreate, db: Session = Depends(get_db)):
    task = tasks_service.get_task(db, task_id)
    if not task:
        raise HTTPException(status_code=404, detail="Task not found")
    return tasks_service.update_task(db, task, payload)


@routers_tasks.patch(
    "/{task_id}",
    response_model=schemas.TaskOut,
    operation_id="tasks_patch_update",
    summary="Частично обновить задачу"
)
def patch_task_endpoint(task_id: int, payload: schemas.TaskUpdate, db: Session = Depends(get_db)):
    task = tasks_service.get_task(db, task_id)
    if not task:
        raise HTTPException(status_code=404, detail="Task not found")
    return tasks_service.update_task(db, task, payload)


@routers_tasks.delete(
    "/{task_id}",
    status_code=status.HTTP_204_NO_CONTENT,
    operation_id="tasks_delete",
    summary="Удалить задачу",
    responses={
        204: {
            "description": "Успешно удалена"
        }}
)
def delete_task_endpoint(task_id: int, db: Session = Depends(get_db)):
    task = tasks_service.get_task(db, task_id)
    if not task:
        raise HTTPException(status_code=404, detail="Task not found")
    tasks_service.delete_task(db, task)
