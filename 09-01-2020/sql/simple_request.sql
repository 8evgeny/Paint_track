CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

select images_content 
FROM images 
WHERE images_ID = 1;

-- select * from object_point WHERE object_point_parrent = 5 AND object_point_time < 200  order by object_point_time desc limit 1;
-- 
-- select * from object_point WHERE object_point_parrent = 5 AND object_point_time > 200 limit 1;






