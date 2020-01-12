CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE 
IF NOT EXISTS 
images(
images_id SERIAL PRIMARY KEY,
images_content bytea,
images_time INT 
);



