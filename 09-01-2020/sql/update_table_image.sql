CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

DROP TABLE 
images; 

CREATE TABLE 
IF NOT EXISTS 
images(
images_id SERIAL PRIMARY KEY,
-- images_content bytea,
images_content VARCHAR(200),
images_time INT 
);

INSERT INTO images ( images_content , images_time ) VALUES 
('/home/eparubets/sadko/src/desktop/poligon_example/sql/data/image1.jpg',0 );
INSERT INTO images ( images_content , images_time ) VALUES 
('/home/eparubets/sadko/src/desktop/poligon_example/sql/data/image2.jpg',1 );
INSERT INTO images ( images_content , images_time ) VALUES 
('/home/eparubets/sadko/src/desktop/poligon_example/sql/data/image3.jpg',2 );
INSERT INTO images ( images_content , images_time ) VALUES 
('/home/eparubets/sadko/src/desktop/poligon_example/sql/data/image4.jpg',3 );
INSERT INTO images ( images_content , images_time ) VALUES 
('/home/eparubets/sadko/src/desktop/poligon_example/sql/data/image5.jpg',4 );
INSERT INTO images ( images_content , images_time ) VALUES 
('/home/eparubets/sadko/src/desktop/poligon_example/sql/data/image6.jpg',5 );
INSERT INTO images ( images_content , images_time ) VALUES 
('/home/eparubets/sadko/src/desktop/poligon_example/sql/data/image7.jpg',6 );
INSERT INTO images ( images_content , images_time ) VALUES 
('/home/eparubets/sadko/src/desktop/poligon_example/sql/data/image8.jpg',7 );
INSERT INTO images ( images_content , images_time ) VALUES 
('/home/eparubets/sadko/src/desktop/poligon_example/sql/data/image9.jpg',8 );
INSERT INTO images ( images_content , images_time ) VALUES 
('/home/eparubets/sadko/src/desktop/poligon_example/sql/data/image10.jpg',9 );

-- SELECT images_content FROM images  WHERE images_time = 500;
