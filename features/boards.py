#!/usr/bin/env python3.10
# -*- coding: utf-8 -*-

from sqlalchemy.orm import Session
from features.ORM_models import *
from features.schemas import *
from features.database import *

Base.metadata.create_all(bind=engine)

def list_boards(db: Session) -> list[Boards]:
    return db.query(Boards).all()

def get_board(db: Session, board_id: int) -> Boards | None:
    return db.query(Boards).get(board_id)

def create_board(db: Session, data: BoardCreate) -> Boards:
    obj = Boards(**data.model_dump())
    db.add(obj)
    db.commit()
    db.refresh(obj)
    return obj

def update_board(db: Session, board: Boards, data: BoardUpdate) -> Boards:
    for k, v in data.model_dump(exclude_unset=True).items():
        setattr(board, k, v)
    db.commit()
    db.refresh(board)
    return board

def delete_board(db: Session, board: Boards) -> None:
    db.delete(board)
    db.commit()
