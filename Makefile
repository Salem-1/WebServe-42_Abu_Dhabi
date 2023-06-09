IMAGE_NAME = my-app
CONTAINER_NAME = my-app-container

.PHONY: build run stop clean

build:
	docker build -t $(IMAGE_NAME) $(PWD)

up:
	docker-compose up -d
run:
	docker-compose run --rm app
stop:
	docker-compose down

clean:
	docker rmi $(IMAGE_NAME)

add-to-path:
	echo "alias my-app='docker-compose run --rm app'" >> ~/.zshrc && source ~/.zshrc
