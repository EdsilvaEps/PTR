clear all

ref_data = load('output.ascii');
refx = ref_data(:,1);
refy = ref_data(:,2);

t_120 = 1:0.120:20;
t_30 = 1:0.03:20;

%  Refx, Refy, xc, yc, Theta, y1, y2 

% printando Referencia
f = figure();
plot(t_120, refx(1:length(t_120)));
grid on 
hold on
plot(t_120, refy(1:length(t_120)));
xlabel('tempo em segundos');
legend('ref x', 'ref y');
title('Referencia');

% printando x,y
output_data = load('robo_output.txt');
xc = output_data(:,1);
yc = output_data(:,2);
theta = output_data(:,3);
y1 = output_data(:,4);
y2 = output_data(:,5);

h = figure();
plot(t_30, yc(1:length(t_30)));
hold on
grid on
xlabel('tempo em segundos');
title('yc x t');

q = figure();
plot(t_30, xc(1:length(t_30)));
hold on
grid on
xlabel('tempo em segundos');
title('xc x t');

k = figure();
plot(t_30, theta(1:length(t_30)));
hold on
grid on
xlabel('tempo em segundos');
title('theta x t');

j = figure();
plot(xc, yc);
hold on
grid on
xlabel('xc');
ylabel('yc');
title('xc x yc');

o = figure();
plot(t_30, y1(1:length(t_30)));
hold on
grid on
xlabel('tempo em segundos');
title('y1 x t');

e = figure();
plot(t_30, y2(1:length(t_30)));
hold on
grid on
xlabel('tempo em segundos');
title('y2 x t');

m = figure();
plot(y1, y2);
hold on
grid on
title('y1 x y2');
%saveas(f, 'referencia.jpg');