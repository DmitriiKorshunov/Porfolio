FROM python:3.9

#
WORKDIR /code

#
COPY Deploy/requirements.txt /code/requirements.txt

#
RUN pip install --no-cache-dir --upgrade -r /code/requirements.txt

#
COPY ./Deploy /code/Deploy

EXPOSE 200

#
CMD ["uvicorn", "Deploy.main:app", "--host", "0.0.0.0", "--port", "200"]