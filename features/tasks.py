#!/usr/bin/env python3.10
# -*- coding: utf-8 -*-

from typing import Optional
from sqlalchemy.orm import Session
from features.schemas import *
from features.ORM_models import *
from features.database import *


class BoardNotFound(Exception):
    pass

class TaskNotFound(Exception):
    pass

def list_tasks_for_board(db: Session, board_id: int) -> list[Tasks]:
    return db.query(Tasks).filter(Tasks.board_id == board_id).all()

def get_task(db: Session, task_id: int) -> Optional[Tasks]:
    return db.query(Tasks).get(task_id)

def create_task(db: Session, board_id: int, data: TaskCreate) -> Tasks:
    board = db.query(Boards).get(board_id)
    if not board:
        raise BoardNotFound()

    obj = Tasks(board_id=board_id, **data.model_dump())
    db.add(obj)
    db.commit()
    db.refresh(obj)
    return obj

def update_task(db: Session, task: Tasks, data: TaskUpdate | TaskCreate) -> Tasks:
    # exclude_unset=True  - поддерживает и PUT (полный объект), и PATCH (частичный)
    updates = data.model_dump(exclude_unset=True)
    for k, v in updates.items():
        setattr(task, k, v)
    db.commit()
    db.refresh(task)
    return task

def delete_task(db: Session, task: Tasks) -> None:
    db.delete(task)
    db.commit()