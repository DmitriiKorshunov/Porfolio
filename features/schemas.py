#!/usr/bin/env python3.10
# -*- coding: utf-8 -*-

from datetime import datetime
from typing import Literal, Optional
from pydantic import BaseModel
from pydantic import ConfigDict

TaskStatus = Literal["new", "in_progress", "done"]

class BoardBase(BaseModel):
    name: str
    description: Optional[str] = None

class BoardCreate(BoardBase):
    pass

class BoardUpdate(BaseModel):
    name: Optional[str] = None
    description: Optional[str] = None

class BoardOut(BoardBase):
    id: int
    created_at: datetime
    model_config = ConfigDict(from_attributes=True)

class TaskBase(BaseModel):
    title: str
    description: Optional[str] = None
    status: TaskStatus = "new"

class TaskCreate(TaskBase):
    pass

class TaskUpdate(BaseModel):
    title: Optional[str] = None
    description: Optional[str] = None
    status: Optional[TaskStatus] = None

class TaskOut(TaskBase):
    id: int
    board_id: int
    created_at: datetime
    model_config = ConfigDict(from_attributes=True)
