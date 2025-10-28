import json
import sys
import os.path
import uuid
import logging
import PyPDF2
from PyQt5 import QtGui, QtCore
from PyQt5.QtGui import QPainter, QPdfWriter, QPageSize
from PyQt5.QtWidgets import QWidget, QApplication
import contextlib
import pprint


class MakePDF(QWidget):
    def __init__(self, doc_json):
        print(f"Class MakePDF input data:")
        pprint.pprint(doc_json)
        try:

            super().__init__()
            self.doc_json = doc_json
            self.layout_height = 12500
            self.layout_width = 8500
            self.writer = QPdfWriter(f"./resolution_files/{self.doc_json['uuid_doc']}.pdf")
            self.writer.setPageSize(QPageSize(QPageSize.A4))
            self.writer.setPageMargins(QtCore.QMarginsF(50, 50, 50, 50))
            painter = QPainter(self.writer)
            painter.begin(self)

            pen = QtGui.QPen(QtGui.QBrush(QtGui.QBrush(QtGui.QColor(0, 0, 0))), 10)
            pen.setWidth(15)
            pen.setColor(QtGui.QColor(0, 0, 0))
            painter.setPen(pen)

            h0 = int(self.layout_height * 0.12)
            h1 = int(self.layout_height * 0.30)
            h2 = int(self.layout_height * 0.40)
            h3 = int(self.layout_height * 0.18)

            painter.drawRect(0, h0, self.layout_width, h1)
            painter.drawRect(0, h0 + h1, self.layout_width, h2)
            painter.drawRect(0, h0 + h1 + h2, int(self.layout_width / 2), h3)
            painter.drawRect(int(self.layout_width / 2), h0 + h1 + h2, int(self.layout_width / 2), h3)

            painter.setFont(QtGui.QFont("Times", 14, 50))

            title_rect = QtCore.QRect(0, 0, self.layout_width, int(self.layout_height * 0.15))
            painter.drawText(title_rect, QtCore.Qt.AlignmentFlag.AlignLeft | QtCore.Qt.AlignmentFlag.AlignTop,
                             "Карточка документа от   __________________  №   _______________________________\n\n"
                             "От кого поступил     _________________________________________________________\n\n"
                             "Исходящий от    _________________________  №   ________________________________\n\n")

            painter.setFont(QtGui.QFont("Times", 14, 75))

            temp_rect = QtCore.QRect(0, h0, self.layout_width, int(h1 * 0.07))
            painter.drawText(temp_rect, QtCore.Qt.AlignmentFlag.AlignCenter | QtCore.Qt.AlignmentFlag.AlignBottom |
                             QtCore.Qt.TextFlag.TextDontClip | QtCore.Qt.TextFlag.TextWordWrap,
                             "Резолюция начальника института")

            temp_rect = QtCore.QRect(0, h0 + h1, self.layout_width, int(h2 * 0.07))
            painter.drawText(temp_rect, QtCore.Qt.AlignmentFlag.AlignCenter | QtCore.Qt.AlignmentFlag.AlignBottom |
                             QtCore.Qt.TextFlag.TextDontClip | QtCore.Qt.TextFlag.TextWordWrap, "Резолюция исполнителей")

            temp_rect = QtCore.QRect(0, h0 + h1 + h2, int(self.layout_width / 2), int(h3 * 0.13))
            painter.drawText(temp_rect, QtCore.Qt.AlignmentFlag.AlignCenter | QtCore.Qt.AlignmentFlag.AlignBottom |
                             QtCore.Qt.TextFlag.TextDontClip | QtCore.Qt.TextFlag.TextWordWrap, "Отметка об исполнении")

            temp_rect = QtCore.QRect(int(self.layout_width / 2), h0 + h1 + h2, int(self.layout_width / 2), int(h3 * 0.13))
            painter.drawText(temp_rect, QtCore.Qt.AlignmentFlag.AlignCenter | QtCore.Qt.AlignmentFlag.AlignBottom |
                             QtCore.Qt.TextFlag.TextDontClip | QtCore.Qt.TextFlag.TextWordWrap, "Особые отметки")

            painter.setFont(QtGui.QFont("Times", 14, 50))

            if "local_date" in doc_json:
                temp_rect = QtCore.QRect(int(self.layout_width * 0.32), 0, int(self.layout_width * 0.3), int(h0 * 0.18))
                painter.drawText(temp_rect, QtCore.Qt.AlignmentFlag.AlignLeft | QtCore.Qt.AlignmentFlag.AlignBottom |
                                 QtCore.Qt.TextFlag.TextDontClip | QtCore.Qt.TextFlag.TextWordWrap, doc_json["local_date"])

            if "local_number" in doc_json:
                temp_rect = QtCore.QRect(int(self.layout_width * 0.66), 0, int(self.layout_width * 0.3), int(h0 * 0.18))
                painter.drawText(temp_rect, QtCore.Qt.AlignmentFlag.AlignLeft | QtCore.Qt.AlignmentFlag.AlignBottom |
                                 QtCore.Qt.TextFlag.TextDontClip | QtCore.Qt.TextFlag.TextWordWrap,
                                 doc_json["local_number"])

            if "sender" in doc_json:
                temp_rect = QtCore.QRect(int(self.layout_width * 0.26), int(h0 * 0.36),
                                         int(self.layout_width * 0.7), int(h0 * 0.2))
                painter.drawText(temp_rect, QtCore.Qt.AlignmentFlag.AlignLeft | QtCore.Qt.AlignmentFlag.AlignBottom |
                                 QtCore.Qt.TextFlag.TextDontClip | QtCore.Qt.TextFlag.TextWordWrap, doc_json["sender"])

            if "in_date" in doc_json:
                temp_rect = QtCore.QRect(int(self.layout_width * 0.21), int(h0 * 0.7),
                                         int(self.layout_width * 0.3), int(h0 * 0.22))
                painter.drawText(temp_rect, QtCore.Qt.AlignmentFlag.AlignLeft | QtCore.Qt.AlignmentFlag.AlignBottom |
                                 QtCore.Qt.TextFlag.TextDontClip | QtCore.Qt.TextFlag.TextWordWrap, doc_json["in_date"])

            if "in_number" in doc_json:
                temp_rect = QtCore.QRect(int(self.layout_width * 0.66), int(h0 * 0.7),
                                         int(self.layout_width * 0.3), int(h0 * 0.22))
                painter.drawText(temp_rect, QtCore.Qt.AlignmentFlag.AlignLeft | QtCore.Qt.AlignmentFlag.AlignBottom |
                                 QtCore.Qt.TextFlag.TextDontClip | QtCore.Qt.TextFlag.TextWordWrap,
                                 doc_json["in_number"])

            if "resolution" in doc_json:
                resolution = doc_json["resolution"]
                painter.setFont(QtGui.QFont("Times", 12, 50))
                if doc_json["nii_head_uuid"] in resolution:
                    resolution_space = QtCore.QRect(0, h0, self.layout_width, h1)

                    head_scale = 0.15
                    signature_space = QtCore.QRectF(resolution_space.x() +
                                                    (resolution_space.width() - resolution_space.width() * head_scale) / 2,
                                                    resolution_space.y() + resolution_space.height() -
                                                    resolution_space.width() * head_scale,
                                                    resolution_space.width() * head_scale,
                                                    resolution_space.width() * head_scale)

                    painter.drawText(resolution_space, QtCore.Qt.AlignmentFlag.AlignCenter |
                                     QtCore.Qt.AlignmentFlag.AlignBottom |
                                     QtCore.Qt.TextFlag.TextDontClip |
                                     QtCore.Qt.TextFlag.TextWordWrap,
                                     resolution[doc_json["nii_head_uuid"]])

                    painter.drawImage(signature_space,
                                      QtGui.QImage(doc_json["signatures_path"] + doc_json["nii_head_uuid"]))
                    del resolution[doc_json["nii_head_uuid"]]

                x = 0
                y = int(h0 + h1 + h2 * 0.1)
                x_length = self.layout_width
                y_length = int(h2 * 0.9)

                n_resolutions = len(resolution)
                half_resolutions = n_resolutions // 2 + n_resolutions % 2
                i = 0
                painter.setFont(QtGui.QFont("Times", 14 - n_resolutions, 50))
                for key in resolution.keys():
                    if n_resolutions <= 3:
                        resolution_space = QtCore.QRect(int(x + (x_length / n_resolutions) * i), y,
                                                        int(x_length / n_resolutions),
                                                        y_length)
                    elif i < half_resolutions:
                        resolution_space = QtCore.QRect(int(x + (x_length / half_resolutions) * i),
                                                        y,
                                                        int(x_length / half_resolutions),
                                                        int(y_length / 2))
                    else:
                        resolution_space = QtCore.QRect(int(x + (x_length / half_resolutions) * (i - half_resolutions)),
                                                        int(y + y_length / 2),
                                                        int(x_length / half_resolutions),
                                                        int(y_length / 2))

                    painter.drawText(resolution_space, QtCore.Qt.AlignmentFlag.AlignCenter |
                                     QtCore.Qt.AlignmentFlag.AlignBottom |
                                     QtCore.Qt.TextFlag.TextDontClip |
                                     QtCore.Qt.TextFlag.TextWordWrap,
                                     resolution[key])
                    scale = 0.3
                    signature_space = QtCore.QRectF(resolution_space.x() +
                                                    (resolution_space.width() - resolution_space.width() * scale) / 2,
                                                    resolution_space.y() + resolution_space.height() -
                                                    resolution_space.width() * scale,
                                                    resolution_space.width() * scale,
                                                    resolution_space.width() * scale)
                    painter.drawImage(signature_space,
                                      QtGui.QImage(doc_json["signatures_path"] + key))
                    i += 1

            if "special_notes" in doc_json:
                special_notes = doc_json["special_notes"]
                if not len(special_notes) == 0:
                    if not len(special_notes[0]) == 0:
                        x = int(self.layout_width / 2 + self.layout_width / 2 * 0.05)
                        y = int(h0 + h1 + h2 + h3 * 0.13)
                        x_length = int(self.layout_width / 2 * 0.9)
                        y_length = int(h3 * 0.85)

                        pen = QtGui.QPen(QtGui.QBrush(QtGui.QBrush(QtGui.QColor(255, 0, 0))), 10)
                        pen.setWidth(25)

                        painter.setFont(QtGui.QFont("Times", 10, 50))
                        painter.setPen(pen)

                        n_special_notes = len(special_notes)
                        half_special_notes = n_special_notes // 2 + n_special_notes % 2

                        for i in range(n_special_notes):
                            if n_special_notes <= 3:
                                special_notes_space = QtCore.QRect(int(x),
                                                                   int(y + y_length / n_special_notes * i),
                                                                   int(x_length),
                                                                   int(y_length / n_special_notes))
                            elif i < half_special_notes:
                                special_notes_space = QtCore.QRect(int(x),
                                                                   int(y + y_length / half_special_notes * i),
                                                                   int(x_length / 2),
                                                                   int(y_length / half_special_notes))
                            else:
                                special_notes_space = QtCore.QRect(int(x + x_length / 2),
                                                                   int(y + y_length / half_special_notes * (
                                                                           i - half_special_notes)),
                                                                   int(x_length / 2),
                                                                   int(y_length / half_special_notes))

                            painter.drawText(special_notes_space, QtCore.Qt.AlignmentFlag.AlignCenter |
                                             QtCore.Qt.TextFlag.TextDontClip |
                                             QtCore.Qt.TextFlag.TextWordWrap,
                                             special_notes[i])
                            painter.drawRect(special_notes_space)

                if "execution_mark" in doc_json and doc_json["execution_mark"]:
                    execution_mark = QtCore.QRect(int(self.layout_width / 2 * 0.05),
                                                  int(h0 + h1 + h2 + h3 * 0.13),
                                                  int(self.layout_width / 2 * 0.9),
                                                  int(h3 * 0.85))

                    pen = QtGui.QPen(QtGui.QBrush(QtGui.QBrush(QtGui.QColor(100, 0, 255))), 10)
                    pen.setWidth(35)
                    painter.setPen(pen)

                    painter.setFont(QtGui.QFont("Times", 14, 75))
                    painter.drawText(execution_mark, QtCore.Qt.AlignmentFlag.AlignTop |
                                     QtCore.Qt.AlignmentFlag.AlignHCenter |
                                     QtCore.Qt.TextFlag.TextDontClip |
                                     QtCore.Qt.TextFlag.TextWordWrap,
                                     "\nИ С П О Л Н Е Н О")

                    painter.setFont(QtGui.QFont("Times", 10, 50))
                    painter.drawText(execution_mark, QtCore.Qt.AlignmentFlag.AlignCenter |
                                     QtCore.Qt.AlignmentFlag.AlignHCenter,
                                     "\n\nИсходящий №   ______________________\n\n "
                                     "Дата исходящего:   ___________________\n")
                    painter.drawRect(execution_mark)

                    pen = QtGui.QPen(QtGui.QBrush(QtGui.QBrush(QtGui.QColor(0, 0, 0))), 10)
                    pen.setWidth(35)
                    painter.setPen(pen)

                    painter.setFont(QtGui.QFont("Times", 10, 50))

                    if "execution_mark_number" in doc_json:
                        execution_mark = QtCore.QRect(int(self.layout_width / 2 * 0.42),
                                                      int(h0 + h1 + h2 + h3 * 0.13),
                                                      int(self.layout_width / 2 * 0.9),
                                                      int(h3 * 0.42))
                        painter.drawText(execution_mark, QtCore.Qt.AlignmentFlag.AlignLeft |
                                         QtCore.Qt.AlignmentFlag.AlignBottom,
                                         doc_json["execution_mark_number"])

                    if "execution_mark_date" in doc_json:
                        execution_mark = QtCore.QRect(int(self.layout_width / 2 * 0.48),
                                                      int(h0 + h1 + h2 + h3 * 0.13),
                                                      int(self.layout_width / 2 * 0.9),
                                                      int(h3 * 0.6))
                        painter.drawText(execution_mark, QtCore.Qt.AlignmentFlag.AlignLeft |
                                         QtCore.Qt.AlignmentFlag.AlignBottom,
                                         doc_json["execution_mark_date"])

            painter.end()
        except:
            return False

def create_new_pdf(json_pdf, uuid_doc, id_process):
        logging.info(
            f'id_process: {id_process}  -> Start process create_new_pdf for document: {uuid_doc}')
        app = QApplication(sys.argv)
        s = MakePDF(doc_json=json_pdf)
        app.closeAllWindows()
        pdf_files_list = [f"./resolution_files/{json_pdf['uuid_doc']}.pdf",
                          f"./clients_files/{json_pdf['uuid_doc']}.pdf"]
        with contextlib.ExitStack() as stack:
            pdf_merger = PyPDF2.PdfFileMerger()
            files = [stack.enter_context(open(pdf, 'rb')) for pdf in pdf_files_list]
            for f in files:
                pdf_merger.append(f)
            with open(f"./gen_pdf_files/{json_pdf['uuid_doc']}.pdf", "wb") as f:
                pdf_merger.write(f)
        logging.info(f'id_process: {id_process}  -> Create pdf-document: {uuid_doc}')
        return True


#
