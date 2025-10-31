#!/usr/bin/env python3.10
# -*- coding: utf-8 -*-

import os
from sqlalchemy import create_engine, event
from sqlalchemy.orm import sessionmaker, DeclarativeBase

os.makedirs("./databases", exist_ok=True)  # гарантируем каталог

class Base(DeclarativeBase):
    pass

engine = create_engine(
    "sqlite:///./databases/dev.db",
    connect_args={"check_same_thread": False},
)

@event.listens_for(engine, "connect")
def set_sqlite_pragma(dbapi_conn, _):
    # Не влияет на создание файла, но важно для FK/каскадов в SQLite
    cur = dbapi_conn.cursor()
    cur.execute("PRAGMA foreign_keys=ON")
    cur.close()

SessionLocal = sessionmaker(bind=engine, autoflush=False, autocommit=False)