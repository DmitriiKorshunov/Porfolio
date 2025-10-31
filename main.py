#!/usr/bin/env python3.10
# -*- coding: utf-8 -*-

from fastapi import FastAPI
from features.ORM_models import *
from features.schemas import *
from features.database import *
from features.routers import routers_tasks, routers_boards

app = FastAPI()
app.include_router(routers_boards)
app.include_router(routers_tasks)
