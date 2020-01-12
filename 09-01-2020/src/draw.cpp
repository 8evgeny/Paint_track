//#include <cmath>
#include "_main.h"
using namespace std;

int MainWind::IsPointInside(int track) {
  pt point;
  for (int number = 1; number != 4; ++number) {
    point = Point_start(track);
    x_region.clear();
    y_region.clear();
    auto connection_string = "host=127.0.0.1 dbname=eparubets user=eparubets";
    pqxx::connection request(connection_string);
    request.prepare(
        "select_points_region",
        "select * from region_point WHERE region_point_parrent = $1;");
    pqxx::nontransaction work(request);
    pqxx::result res1 = work.exec_prepared("select_points_region", number);
    for (const auto &regionpoint : res1) {
      x_region.push_back(regionpoint[1].as<int>());
      y_region.push_back(regionpoint[2].as<int>());
    }
    //  point  x_region  y_region

    // Start algorithm:

    if (x_region.size() <= 1) return -1;

    int intersections_num = 0;
    int prev = x_region.size() - 1;
    bool prev_under = y_region[prev] < point.y;

    for (int i = 0; i < x_region.size(); ++i) {
      bool cur_under = y_region[i] < point.y;
      pt a;
      pt b;
      a.x = x_region[prev] - point.x;
      a.y = y_region[prev] - point.y;
      b.x = x_region[i] - point.x;
      b.y = y_region[i] - point.y;

      float t = (a.x * (b.y - a.y) - a.y * (b.x - a.x));
      if (cur_under && !prev_under) {
        if (t > 0) intersections_num += 1;
      }
      if (!cur_under && prev_under) {
        if (t < 0) intersections_num += 1;
      }

      prev = i;
      prev_under = cur_under;
    }
    if (intersections_num == 1) return number;
  }
  return 0;
}

MainWind::pt MainWind::Point_start(int track) {
  min_t.clear();
  auto connection_string = "host=127.0.0.1 dbname=eparubets user=eparubets";
  pqxx::connection request(connection_string);
  request.prepare("request",
                  "select object_point_x,object_point_y from object_point "
                  "WHERE object_point_time = (select MIN(object_point_time) AS "
                  "object_point_time from object_point WHERE "
                  "object_point_parrent = $1) AND object_point_parrent = $1;");
  pqxx::nontransaction work(request);
  pqxx::result res = work.exec_prepared("request", track);
  for (const auto &i : res) {
    min_t.push_back(i[0].as<int>());
    min_t.push_back(i[1].as<int>());
  }
  start_point.x = min_t[0];
  start_point.y = min_t[1];
  return start_point;
}

bool MainWind::intersect(pt &start1, pt &end1, pt &start2, pt &end2) {
  pt dir1 = end1 - start1;
  pt dir2 = end2 - start2;

  //считаем уравнения прямых проходящих через отрезки
  float a1 = -dir1.y;
  float b1 = +dir1.x;
  float d1 = -(a1 * start1.x + b1 * start1.y);

  float a2 = -dir2.y;
  float b2 = +dir2.x;
  float d2 = -(a2 * start2.x + b2 * start2.y);

  //подставляем концы отрезков, для выяснения в каких полуплоскотях они
  float seg1_line2_start = a2 * start1.x + b2 * start1.y + d2;
  float seg1_line2_end = a2 * end1.x + b2 * end1.y + d2;

  float seg2_line1_start = a1 * start2.x + b1 * start2.y + d1;
  float seg2_line1_end = a1 * end2.x + b1 * end2.y + d1;

  //если концы одного отрезка имеют один знак, значит он в одной полуплоскости и
  //пересечения нет.
  if (seg1_line2_start * seg1_line2_end >= 0 ||
      seg2_line1_start * seg2_line1_end >= 0)
    return false;

  return true;
}

vector<int> &MainWind::valuesMinMax(int parrent) {
  try {
    ValuesMinMax.clear();
    auto connection_string = "host=127.0.0.1 dbname=eparubets user=eparubets";
    pqxx::connection request1(connection_string);
    request1.prepare(
        "select_min",
        "select * from object_point WHERE object_point_parrent = $2 AND "
        "object_point_time < $1  order by object_point_time desc limit 1");
    pqxx::nontransaction work1(request1);
    pqxx::result res1 =
        work1.exec_prepared("select_min", TimeTrajectory, parrent);
    ValuesMinMax.clear();
    for (const auto &i : res1) {
      ValuesMinMax.push_back(i[1].as<int>());  // x min
      ValuesMinMax.push_back(i[2].as<int>());  // y min
      ValuesMinMax.push_back(i[3].as<int>());  // time min
    }

    pqxx::connection request2(connection_string);
    request2.prepare(
        "select_min",
        "select * from object_point WHERE object_point_parrent = $2 AND "
        "object_point_time > $1 limit 1");
    pqxx::nontransaction work2(request2);
    pqxx::result res2 =
        work2.exec_prepared("select_min", TimeTrajectory, parrent);

    for (const auto &i : res2) {
      ValuesMinMax.push_back(i[1].as<int>());  // x max
      ValuesMinMax.push_back(i[2].as<int>());  // y max
      ValuesMinMax.push_back(i[3].as<int>());  // time max
    }

    //    cout << "TimeTrajectory= " << TimeTrajectory
    //         << " min Time =" << ValuesMinMax[2] << " x_min = " <<
    //         ValuesMinMax[0]
    //         << " y_min = " << ValuesMinMax[1] << endl;
    //    cout << "TimeTrajectory= " << TimeTrajectory
    //         << " max Time =" << ValuesMinMax[5] << " x_max = " <<
    //         ValuesMinMax[3]
    //         << " y_max = " << ValuesMinMax[4] << endl;

    //      ValuesMinMax[0]  - x min
    //      ValuesMinMax[1]  - y min
    //      ValuesMinMax[2]  - t min
    //      ValuesMinMax[3]  - x max
    //      ValuesMinMax[4]  - y max
    //      ValuesMinMax[5]  - t max

  }  // end try
  catch (const pqxx::sql_error &e) {
    std::cout << "SQL error: " << e.what();
    std::cout << "Query was: '" << e.query().c_str() << "'";
  }

  return ValuesMinMax;
}

void MainWind::draw_track(int num_track, vector<int> &x, vector<int> &y,
                          vector<int> &color, vector<int> &x1, vector<int> &y1,
                          vector<int> &x2, vector<int> &y2, vector<int> &x3,
                          vector<int> &y3) {
  try {
    int color_track = IsPointInside(num_track);

    if (scenary >= 2) {  // Approksimation
      if (requestValues_inTime(num_track) == -1) {
        painter.setPen(QPen(Qt::white, 6));
        valuesMinMax(num_track);
        //        cout << "TimeTrajectory= " << TimeTrajectory
        //             << " min Time =" << ValuesMinMax[2]
        //             << " x_min = " << ValuesMinMax[0] << " y_min = " <<
        //             ValuesMinMax[1]
        //             << endl;
        //        cout << "TimeTrajectory= " << TimeTrajectory
        //             << " max Time =" << ValuesMinMax[5]
        //             << " x_max = " << ValuesMinMax[3] << " y_max = " <<
        //             ValuesMinMax[4]
        //             << endl;
        //      ValuesMinMax[0]  - x min
        //      ValuesMinMax[1]  - y min
        //      ValuesMinMax[2]  - t min
        //      ValuesMinMax[3]  - x max
        //      ValuesMinMax[4]  - y max
        //      ValuesMinMax[5]  - t max
        double x_ = 0;
        double y_ = 0;
        double coeff = ((double)TimeTrajectory - (double)ValuesMinMax[2]) /
                       ((double)ValuesMinMax[5] - (double)ValuesMinMax[2]);
        //        cout << ValuesMinMax[2] << endl;
        x_ = coeff * ((double)ValuesMinMax[3] - (double)ValuesMinMax[0]) +
             (double)ValuesMinMax[0];
        y_ = coeff * ((double)ValuesMinMax[4] - (double)ValuesMinMax[1]) +
             (double)ValuesMinMax[1];
        painter.drawPoint(x_, y_);
      }
    }  // end approcsimation

    // set begin color painter
    if (color_track == 0) painter.setPen(QPen(Qt::white, 3));
    if (color_track == 1) painter.setPen(QPen(Qt::blue, 3));
    if (color_track == 2) painter.setPen(QPen(Qt::red, 3));
    if (color_track == 3) painter.setPen(QPen(Qt::yellow, 3));

    auto connection_string = "host=127.0.0.1 dbname=eparubets user=eparubets";
    pqxx::connection request(connection_string);
    request.prepare("select_points_object1",
                    "select * from object_point WHERE object_point_time <= $1 "
                    "AND object_point_parrent = $2;");
    pqxx::nontransaction work(request);
    pqxx::result res =
        work.exec_prepared("select_points_object1", TimeTrajectory, num_track);

    for (const auto &objectpoint : res) {
      x.push_back(objectpoint[1].as<int>());
      y.push_back(objectpoint[2].as<int>());
      color.push_back(color_track);
    }

    pt pt1;
    pt pt2;
    pt pt3;
    pt pt4;

    for (unsigned long k = 2; k < x.size() - 1; ++k) {
      pt1.x = x[k - 1];
      pt1.y = y[k - 1];
      pt2.x = x[k + 1];
      pt2.y = y[k + 1];

      for (unsigned long i = 1; i <= x1.size() - 1; ++i) {
        if (i < x1.size() - 1) {
          pt3.x = x1[i];
          pt3.y = y1[i];
          pt4.x = x1[i + 1];
          pt4.y = y1[i + 1];
        }
        if (i == x1.size() - 1) {
          pt3.x = x1[i];
          pt3.y = y1[i];
          pt4.x = x1[1];
          pt4.y = y1[1];
        }

        if (intersect(pt1, pt2, pt3, pt4)) {
          switch (color[k]) {
            case 0:
              VectorFill(color, k + 2, 1);
              painter.setPen(QPen(Qt::blue, 3));
              break;
            case 1:
              VectorFill(color, k + 2, 0);
              painter.setPen(QPen(Qt::white, 3));
              break;
            case 2:
              VectorFill(color, k + 2, 1);
              painter.setPen(QPen(Qt::blue, 3));
              break;
            case 3:
              VectorFill(color, k + 2, 1);
              painter.setPen(QPen(Qt::blue, 3));
              break;
          }
        }

      }  // end 1-1 region

      for (unsigned long i = 1; i <= x2.size() - 1; ++i) {
        if (i < x2.size() - 1) {
          pt3.x = x2[i];
          pt3.y = y2[i];
          pt4.x = x2[i + 1];
          pt4.y = y2[i + 1];
        }
        if (i == x2.size() - 1) {
          pt3.x = x2[i];
          pt3.y = y2[i];
          pt4.x = x2[1];
          pt4.y = y2[1];
        }

        if (intersect(pt1, pt2, pt3, pt4)) {
          switch (color[k]) {
            case 0:
              VectorFill(color, k + 2, 2);
              painter.setPen(QPen(Qt::red, 3));
              break;
            case 2:
              VectorFill(color, k + 2, 0);
              painter.setPen(QPen(Qt::white, 3));
              break;
            case 1:
              VectorFill(color, k + 2, 2);
              painter.setPen(QPen(Qt::red, 3));
              break;
            case 3:
              VectorFill(color, k + 2, 2);
              painter.setPen(QPen(Qt::red, 3));
              break;
          }
        }

      }  // end 2-1 region

      for (unsigned long i = 1; i <= x3.size() - 1; ++i) {
        if (i < x3.size() - 1) {
          pt3.x = x3[i];
          pt3.y = y3[i];
          pt4.x = x3[i + 1];
          pt4.y = y3[i + 1];
        }
        if (i == x3.size() - 1) {
          pt3.x = x3[i];
          pt3.y = y3[i];
          pt4.x = x3[1];
          pt4.y = y3[1];
        }

        if (intersect(pt1, pt2, pt3, pt4)) {
          switch (color[k]) {
            case 0:
              VectorFill(color, k + 2, 3);
              painter.setPen(QPen(Qt::yellow, 3));
              break;
            case 3:
              VectorFill(color, k + 2, 0);
              painter.setPen(QPen(Qt::white, 3));
              break;
            case 1:
              VectorFill(color, k + 2, 3);
              painter.setPen(QPen(Qt::yellow, 3));
              break;
            case 2:
              VectorFill(color, k + 2, 3);
              painter.setPen(QPen(Qt::yellow, 3));
              break;
          }
        }

      }  // end 3-1 region

      painter.drawPoint(x[k], y[k]);
    }

  }  // end try
  catch (const pqxx::sql_error &e) {
    std::cout << "SQL error: " << e.what();
    std::cout << "Query was: '" << e.query().c_str() << "'";
  }
}

void MainWind::VectorFill(vector<int> &vector, unsigned long poz, int color) {
  for (unsigned long i = poz; i < vector.size(); ++i) {
    vector[i] = color;
  }
}

void MainWind::draw_region(int number, vector<int> &x, vector<int> &y,
                           int color) {
  try {
    auto connection_string = "host=127.0.0.1 dbname=eparubets user=eparubets";
    pqxx::connection request(connection_string);
    request.prepare(
        "select_points_region1",
        "select * from region_point WHERE region_point_parrent = $1;");
    pqxx::nontransaction work(request);
    pqxx::result res3 = work.exec_prepared("select_points_region1", number);
    for (const auto &regionpoint : res3) {
      x.push_back(regionpoint[1].as<int>());
      y.push_back(regionpoint[2].as<int>());
    }

    if (color == 1) painter.setPen(QPen(Qt::blue, 4));
    if (color == 2) painter.setPen(QPen(Qt::red, 4));
    if (color == 3) painter.setPen(QPen(Qt::yellow, 4));
    for (int i = 1; i < x.size() - 1; ++i) {
      painter.drawLine(x[i], y[i], x[i + 1], y[i + 1]);
    }
    painter.drawLine(x[1], y[1], x[x.size() - 1], y[x.size() - 1]);

  }  // end try
  catch (const pqxx::sql_error &e) {
    std::cout << "SQL error: " << e.what();
    std::cout << "Query was: '" << e.query().c_str() << "'";
  }
}

int MainWind::requestValues_inTime(int parrent) {
  try {
    auto connection_string = "host=127.0.0.1 dbname=eparubets user=eparubets";
    pqxx::connection request(connection_string);
    request.prepare("select_points_object1",
                    "select * from object_point WHERE object_point_time = $1 "
                    "AND object_point_parrent = $2 ;");
    pqxx::nontransaction work(request);
    pqxx::result res =
        work.exec_prepared("select_points_object1", TimeTrajectory, parrent);

    for (const auto &i : res) {
      Values_inTime.push_back(i[0].as<int>());  // ID
      //      Values_inTime.push_back(i[1].as<int>());  // x
      //      Values_inTime.push_back(i[2].as<int>());  // y
      //      Values_inTime.push_back(i[3].as<int>());  // t
      //      Values_inTime.push_back(i[4].as<int>());  // parrent
    }
  }  // end try
  catch (const pqxx::sql_error &e) {
    std::cout << "SQL error: " << e.what();
    std::cout << "Query was: '" << e.query().c_str() << "'";
  }
  if (Values_inTime.empty()) {
    return -1;
  }
  Values_inTime.clear();
  return 0;
}

QImage MainWind::selectImagePath(int time) {
  try {
    Patch.clear();
    auto connection_string = "host=127.0.0.1 dbname=eparubets user=eparubets";
    pqxx::connection request(connection_string);
    request.prepare(
        "select_patch",
        "SELECT images_content FROM images  WHERE images_time = $1");
    pqxx::nontransaction work(request);
    pqxx::result res = work.exec_prepared("select_patch", time);
    for (const auto &i : res) {
      Patch.push_back(i[0].as<string>());  // Путь
    }
  }  // end try
  catch (const pqxx::sql_error &e) {
    std::cout << "SQL error: " << e.what();
    std::cout << "Query was: '" << e.query().c_str() << "'";
  }
  string patch = Patch[0];
  QString patch_;
  for (int i = 0; i < patch.size(); ++i) {
    patch_[i] = patch[i];
  }

  image.load(patch_);
  //  QImage image1;
  //  image1 = image.copy(0, 0, 1920, 1080);
  //  image.setPixel(1920, 1080, RGB_MASK);
  return image;
}

void MainWind::renderNow() {
  //  QPainter painter;
  //  auto paintref = &painter;
  //  QPainterPath path;
  painter.begin(this);
  QPalette pall;
  QImage image;

  painter.setRenderHint(QPainter::Antialiasing, true);

  if (scenary == 3) {
    Time_imaje = TimeTrajectory / 200;
    int time = Time_imaje;
    image = selectImagePath(time);

    painter.drawImage(0, 0, image);

  }  // end if (scenary == 3)

  if (scenary == 2) {
    pall.setColor(this->backgroundRole(), Qt::lightGray);
    this->setPalette(pall);
    //    this->setAutoFillBackground(false);
  }
  if (scenary == 1) {
    pall.setColor(this->backgroundRole(), Qt::gray);
    this->setPalette(pall);
    //    this->setAutoFillBackground(false);
  }

  vector<int> x1 = {0};
  vector<int> y1 = {0};
  vector<int> x2 = {0};
  vector<int> y2 = {0};
  vector<int> x3 = {0};
  vector<int> y3 = {0};
  vector<int> zona = {0};
  vector<int> ob_x1 = {0};
  vector<int> ob_y1 = {0};
  vector<int> ob_x2 = {0};
  vector<int> ob_y2 = {0};
  vector<int> ob_x3 = {0};
  vector<int> ob_y3 = {0};
  vector<int> ob_x4 = {0};
  vector<int> ob_y4 = {0};
  vector<int> ob_x5 = {0};
  vector<int> ob_y5 = {0};
  vector<int> color1 = {0};
  vector<int> color2 = {0};
  vector<int> color3 = {0};
  vector<int> color4 = {0};
  vector<int> color5 = {0};

  struct pt {
    int x;
    int y;
  };

  draw_region(1, x1, y1, 1);
  draw_region(2, x2, y2, 2);
  draw_region(3, x3, y3, 3);
  draw_track(1, ob_x1, ob_y1, color1, x1, y1, x2, y2, x3, y3);
  draw_track(2, ob_x2, ob_y2, color2, x1, y1, x2, y2, x3, y3);
  //  draw_track(3, ob_x3, ob_y3, color3, x1, y1, x2, y2, x3, y3);
  draw_track(4, ob_x4, ob_y4, color4, x1, y1, x2, y2, x3, y3);
  draw_track(5, ob_x5, ob_y5, color5, x1, y1, x2, y2, x3, y3);

  painter.end();
}
