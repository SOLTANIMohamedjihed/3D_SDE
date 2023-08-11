#include <ql/quantlib.hpp>

using namespace QuantLib;

int main() {
    try {
        // Définir les paramètres de l'EDS
        Real x0 = 1.0; // Condition initiale pour x
        Real y0 = 2.0; // Condition initiale pour y
        Real z0 = 3.0; // Condition initiale pour z
        Real mu_x = 0.05; // Coefficient de dérive pour x
        Real mu_y = 0.03; // Coefficient de dérive pour y
        Real mu_z = 0.02; // Coefficient de dérive pour z
        Real sigma_x = 0.2; // Coefficient de diffusion pour x
        Real sigma_y = 0.15; // Coefficient de diffusion pour y
        Real sigma_z = 0.1; // Coefficient de diffusion pour z
        Time maturity = 1.0; // Temps final
        Size timeSteps = 100; // Nombre de pas de temps

        // Créer les processus de diffusion
        boost::shared_ptr<StochasticProcess1D> diffusionProcessX(
            new GeometricBrownianMotionProcess(x0, mu_x, sigma_x));
        boost::shared_ptr<StochasticProcess1D> diffusionProcessY(
            new GeometricBrownianMotionProcess(y0, mu_y, sigma_y));
        boost::shared_ptr<StochasticProcess1D> diffusionProcessZ(
            new GeometricBrownianMotionProcess(z0, mu_z, sigma_z));

        // Créer le processus de diffusion en 3D
        boost::shared_ptr<StochasticProcess> diffusionProcess(
            new GeneralizedBlackScholesProcess(
                Handle<Quote>(new SimpleQuote(x0)),
                Handle<YieldTermStructure>(new FlatForward(0, NullCalendar(), 0.0, Actual365Fixed())),
                Handle<YieldTermStructure>(new FlatForward(0, NullCalendar(), 0.0, Actual365Fixed())),
                Handle<BlackVolTermStructure>(new BlackConstantVol(0, NullCalendar(), sigma_x, Actual365Fixed())),
                Handle<BlackVolTermStructure>(new BlackConstantVol(0, NullCalendar(), sigma_y, Actual365Fixed())),
                Handle<BlackVolTermStructure>(new BlackConstantVol(0, NullCalendar(), sigma_z, Actual365Fixed()))
            ));

        // Créer le chemin stochastique en 3D
        TimeGrid grid(maturity, timeSteps);
        boost::shared_ptr<PathGenerator<MultiPath>> pathGenerator(
            new PathGenerator<MultiPath>(diffusionProcess, grid));

        // Générer le chemin stochastique en 3D
        pathGenerator->next();

        // Obtenir les valeurs du chemin stochastique en 3D
        const std::vector<std::vector<Real>>& path = pathGenerator->next().value;

        // Afficher les valeurs du chemin stochastique en 3D
        for (Size i = 0; i < path.size(); ++i) {
            std::cout << "x(" << grid[i] << ") = " << path[i][0] << std::endl;
            std::cout << "y(" << grid[i] << ") = " << path[i][1] << std::endl;
            std::cout << "z(" << grid[i] << ") = " << path[i][2] << std::endl;
        }

        return 0;
    }
    catch (std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }
}