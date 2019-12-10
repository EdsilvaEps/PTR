clear all
data = importdata('output.ascii');
data(5,:)

t = data(:,1);

posx = data(:,4);
posy = data(:,5);
theta = data(:,6);

h = figure();
plot(posx, posy)
hold on
grid on
xlabel('coordenada xc');
ylabel('coordenada yc');

x = figure();
plot(t, posx)
hold on
grid on
plot(t, posy)
plot(t, theta);
xlabel('tempo de simulacao em segundos');
legend('coordenada x', 'coordenada y', 'angulo teta');

saveas(h, 'posicao.jpg')
saveas(x, 'saida.jpg')