clear all
data = importdata('saida.asc');
data(5,:)

t = data(:,1);

posx = data(:,10);
posy = data(:,11);
theta = data(:,12);
linspeed = data(:,8);
angspeed = data(:,9);

h = figure();
plot(posx, posy)
hold on
grid on
xlabel('coordenada xc');
ylabel('coordenada yc');

y = figure();
plot(t, linspeed)
hold on
grid on
plot(t, angspeed);
xlabel('tempo de simulacao em segundos');
legend('velocidade linear', 'velocidade angular');

x = figure();
plot(t, posx)
hold on
grid on
plot(t, posy)
plot(t, theta);
xlabel('tempo de simulacao em segundos');
legend('coordenada x', 'coordenada y', 'angulo teta');

saveas(h, 'posicao.jpg')
saveas(y, 'velocidade.jpg')
saveas(x, 'saida.jpg')
