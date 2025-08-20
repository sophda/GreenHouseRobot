package com.servlet;
import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.db.PhenoDB;


public class SetPhenotype extends HttpServlet{


    public void doGet(HttpServletRequest request,
                      HttpServletResponse response)
        throws ServletException, IOException
    {
        String username = request.getParameter("username");
        String group = request.getParameter("group");
        String phenotype = request.getParameter("phenotype");
        String gps = request.getParameter("gps");
        PhenoDB.setphenotype(username,group,phenotype,gps);
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/html");
        PrintWriter out = response.getWriter();
        out.print("上传完成");
        out.flush();
        out.close();




    }



}
