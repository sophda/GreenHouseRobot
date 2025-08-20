package com.db;

import javax.xml.crypto.Data;
import java.sql.*;

public class PhenoDB {

    // MySQL 8.0 以下版本 - JDBC 驱动名及数据库 URL
    static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";
    static final String DB_URL = "jdbc:mysql://localhost:3306/guocai";

    // 数据库的用户名与密码，需要根据自己的设置
    static final String USER = "root";
    static final String PASS = "123456789";

    public static String getphenotype(String user,String group) {
        Connection conn = null;
        Statement stmt = null;
        String DataOut = ",,,,";
        try{

            // 查询对象
            String usergroup = user + group;

            // 注册 JDBC 驱动
            Class.forName(JDBC_DRIVER);

            // 打开链接
            System.out.println("连接数据库...");
            conn = DriverManager.getConnection(DB_URL,USER,PASS);

            // 执行查询
            //System.out.println(" 实例化Statement对象...");
            stmt = conn.createStatement();
            String sql;
            //sql = "SELECT id, name, url FROM websites";
            sql = "select * from guocaipheno where usergroup = '"+usergroup+"'";
            ResultSet rs = stmt.executeQuery(sql);
            rs.next();
            String phenotype = rs.getString("phenotype");
            String gps = rs.getString("gps");
            //System.out.println(phenotype);
            DataOut = phenotype+"&"+gps;
            rs.close();
            stmt.close();
            conn.close();
            //return DataOut;
        }catch(SQLException se){
            // 处理 JDBC 错误
            se.printStackTrace();
        }catch(Exception e){
            // 处理 Class.forName 错误
            e.printStackTrace();
        }finally{
            // 关闭资源
            try{
                if(stmt!=null) stmt.close();
            }catch(SQLException se2){
            }// 什么都不做
            try{
                if(conn!=null) conn.close();
            }catch(SQLException se){
                se.printStackTrace();
            }
        }
        //System.out.println("Goodbye!");
        //return " , , , ,";
        return DataOut;
    }




public static void setphenotype(String username,
                                String group,
                                String phenotype,
                                String gps)
    {
        Connection conn = null;
        Statement stmt = null;
        try{

            // 查询对象
            String usergroup = username + group;

            // 注册 JDBC 驱动
            Class.forName(JDBC_DRIVER);

            // 打开链接
            System.out.println("连接数据库...");
            conn = DriverManager.getConnection(DB_URL,USER,PASS);

            // 执行查询
            System.out.println(" 实例化Statement对象...");
            stmt = conn.createStatement();

            //String usergroup = username+group;
            String setsql = "insert into guocaipheno (usergroup,phenotype) values('"
                    + usergroup + "','" + phenotype + "') ";
            String logsql = "select * from guocaipheno where usergroup = '"
                    + usergroup+"'";
//            String updatesql = "update guocaipheno set phenotype='"+ phenotype+"'"
//                    +" where usergroup='" + usergroup+"'";
            String updatesql = "update guocaipheno set phenotype='"+ phenotype+"'" + ",gps='"+gps+"'"
                    +" where usergroup='" + usergroup+"'";




            ResultSet rs = stmt.executeQuery(logsql);
            if (rs.next()) {
                int rs_log = stmt.executeUpdate(updatesql);
                System.out.println("update successfully");
            }
            else
            {
                boolean rs_set = stmt.execute(setsql);
                System.out.println("set successfully");

            }


            rs.close();
            stmt.close();
            conn.close();

        }catch(SQLException se){
            // 处理 JDBC 错误
            se.printStackTrace();
        }catch(Exception e){
            // 处理 Class.forName 错误
            e.printStackTrace();
        }finally{
            // 关闭资源
            try{
                if(stmt!=null) stmt.close();
            }catch(SQLException se2){
            }// 什么都不做
            try{
                if(conn!=null) conn.close();
            }catch(SQLException se){
                se.printStackTrace();
            }
        }
        //System.out.println("Goodbye!");
    }
}