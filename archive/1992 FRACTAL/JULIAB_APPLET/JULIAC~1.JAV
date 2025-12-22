import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.Color;
import java.lang.Math;

public class JuliaConstellation extends java.applet.Applet
                implements Runnable {
        int numTraces;     // parameters
        double fa1, fb1,
                fa2, fb2;
        double dt;      // member variables
        JuliaSet js[];

        public void init() {
                numTraces = 2;    // parse the parameters
                fa1 = 0.11;    fa2 = 0.17;
                fb1 = 0.13;    fb2 = 0.16;
                dt = 0;         // initialize state variables
                js = new JuliaSet[numTraces];
        }  // init

        Thread tracer;
        public void run() {
                do {    updateParams();
                        repaint();
                        //paint(getGraphics());
                        try {   tracer.sleep(15);
                        } catch (InterruptedException e) { }
                } while (true);
        }  // run

        public void start() {
                tracer = new Thread(this);
                tracer.start();
        }  // start

        public void stop() {
                tracer = null;
        }  // stop

        public int updateParams() {
                dt += 1.0;
                fa1 += Math.random()*fa1*0.01 - fa1*0.005; // update fa & fb
                fa2 += Math.random()*fa2*0.005 - fa2*0.0025;
                fb1 += Math.random()*fb1*0.01 - fb1*0.005;
                fb2 += Math.random()*fb2*0.005 - fb2*0.0025;
                int at = (int)dt % numTraces;
                js[at] = new JuliaSet(Math.sin(fa1*dt) +
                        Math.sin(fa2*dt)*0.8, Math.cos(fb1*dt) +
                        Math.cos(fb2*dt)*0.8);
                return at;
        }  // updateParams

        public void paint(Graphics g) {
                // g.setXORMode(new Color(224, 224, 255));
                g.setPaintMode();
                // int at = (int)(dt+1) % numTraces;
                // if (js[at] != null)
                //        js[at].plotSet(g);
                int at = (int)dt % numTraces;
                if (js[at] != null)
                        js[at].plotSet(g);
        }  // paint
}  // Fractal

class JuliaSet {
        double a, b;
        int p;
        JuliaSet(double initA, double initB) {
                a = initA; b = initB; p = 5;
                oldx = new int[2]; oldy = new int[2];
        }  // JuliaSet

        int oldx[], oldy[];
        void plotElement(Graphics g, double x, double y, int size,
                        int series) {
                        // (int)Math.abs(y/128), (int)Math.abs(x/128));
                int newx = (int)(x) % 500;
                int newy = (int)(y + 250) % 500;
                if (oldx[series] > 0)
                        g.drawLine(oldx[series], oldy[series], newx, newy);
                oldx[series] = newx;
                oldy[series] = newy;
                g.fillOval(newx, newy, size, size);
        }  // plotElement

        void plotSet(Graphics g) {
                double a1 = (0.25-a) / 2.0;
                double b1 = -b / 2.0;
                double r1 = Math.sqrt(a1*a1 + b1*b1);

                double x1[] = new double[p];
                double y1[] = new double[p];
                double x2[] = new double[p];
                double y2[] = new double[p];
                x1[0] = 0.5+Math.sqrt(r1 + a1);
                y1[0] = Math.sqrt(r1 - a1);
                if (b > 0.0) y1[0] = -y1[0];
                int s = 1;
                for (int j = s; j < p; j++) {
                        double x = x1[j-1];
                        double y = y1[j-1];
                        double r = Math.sqrt((x - a1)*(x - a1) +
                                (y - b1)*(y - b1))/2.0;
                        double t = (x - a1)/2.0;
                        x1[j] = Math.sqrt(r + t);
                        x2[j] = -x1[j];
                        y1[j] = Math.sqrt(r - t);
                        if (y < b1)
                                y1[j] = -y1[j];
                        y2[j] = -y1[j];
                }  // for
                
                oldx[0] = -1; oldx[1] = -1;
                for (int m = 1; m <= (1 << (p-1)); m++) {
                        s = p;
                        int n = m;
                        while (n % 2 == 0) {
                                n /= 2; 
                                s--;
                        }  // while 
                        x1[s-1] = x2[s-1];
                        y1[s-1] = y2[s-1];
                        for (int j = s; j < p; j++) {
                                double x = x1[j-1];
                                double y = y1[j-1];
                                double r = Math.sqrt((x - a1)*(x - a1)+
                                        (y - b1)*(y - b1))/2.0;
                                double t = (x - a1)/2.0;
                                x1[j] = Math.sqrt(r + t);
                                x2[j] = -x1[j];
                                y1[j] = Math.sqrt(r - t);
                                if (y < b1) y1[j] = -y1[j];
                                y2[j] = -y1[j];
                                g.setColor(new Color((m % 19)*5 + 128,
                                        (m % 23)*5 + 128,
                                        (m % 27)*4 + 128));
                                plotElement(g,
                                        ((x1[j]+2.667*4)*150/2),
                                        ((y1[j]+2.000*4)*100/2),
                                        (m % 11)+2, 0);
                                plotElement(g,
                                        ((x2[j]+2.667*4)*150/2),
                                        ((y2[j]+2.000*4)*100/2), 
                                        (m % 11)+2, 1);
                        }  // for
                }  // for
        }  // plotSet
}  // JuliaSet
