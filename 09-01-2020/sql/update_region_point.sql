CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

DROP TABLE 
region_point; 

CREATE TABLE 
IF NOT EXISTS 
region_point(
region_point_id SERIAL PRIMARY KEY,
region_point_x INT, 
region_point_y INT, 
region_point_parrent  INT
);

INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (50,50,1);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (40,850,1);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (300,960,1);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (700,700,1);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (500,100,1);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (300,20,1);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (90,30,1);

INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (500,5,2);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (800,850,2);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (1200,960,2);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (1170,230,2);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (1000,50,2);

INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (1250,50,3);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (1200,100,3);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (1300,850,3);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (1500,960,3);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (1900,700,3);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (1850,100,3);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (1700,20,3);
INSERT INTO region_point (region_point_x, region_point_y, region_point_parrent) VALUES (1290,30,3);
