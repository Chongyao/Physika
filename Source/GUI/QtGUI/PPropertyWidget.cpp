#include "PPropertyWidget.h"
#include "Framework/Module.h"
#include "Framework/Node.h"
#include "Framework/Framework/SceneGraph.h"

#include "PVTKOpenGLWidget.h"
#include "PCustomWidgets.h"
#include "Nodes/QtNodeWidget.h"
#include "Nodes/QtModuleWidget.h"

#include "Common.h"

#include "vtkRenderer.h"
#include <vtkRenderWindow.h>

#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QRegularExpression>
#include <QMessageBox>

namespace PhysIKA
{
	QBoolFieldWidget::QBoolFieldWidget(Field* field)
		: QGroupBox()
	{
		m_field = field;
		VarField<bool>* f = TypeInfo::CastPointerDown<VarField<bool>>(m_field);
		if (f == nullptr)
		{
			return;
		}

		this->setStyleSheet("border:none");
		QGridLayout* layout = new QGridLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		this->setLayout(layout);

		QLabel* name = new QLabel();
		name->setFixedSize(160, 18);
		name->setText(FormatFieldWidgetName(field->getObjectName()));
		QCheckBox* checkbox = new QCheckBox();
		//checkbox->setFixedSize(40, 18);
		layout->addWidget(name, 0, 0);
		layout->addWidget(checkbox, 0, 1);

		connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(changeValue(int)));

		checkbox->setChecked(f->getValue());
	}


	void QBoolFieldWidget::changeValue(int status)
	{
		VarField<bool>* f = TypeInfo::CastPointerDown<VarField<bool>>(m_field);
		if (f == nullptr)
		{
			return;
		}

		if (status == Qt::Checked)
		{
			f->setValue(true);
			f->update();
		}
		else if (status == Qt::PartiallyChecked)
		{
			//m_pLabel->setText("PartiallyChecked");
		}
		else
		{
			f->setValue(false);
			f->update();
		}

		emit fieldChanged();
	}

	QIntegerFieldWidget::QIntegerFieldWidget(Field* field)
		: QGroupBox()
	{
		m_field = field;
		VarField<int>* f = TypeInfo::CastPointerDown<VarField<int>>(m_field);
		if (f == nullptr)
		{
			return;
		}

		this->setStyleSheet("border:none");
		QGridLayout* layout = new QGridLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		this->setLayout(layout);

		QLabel* name = new QLabel();
		name->setFixedSize(160, 18);
		name->setText(FormatFieldWidgetName(field->getObjectName()));

		QSpinBox* spinner = new QSpinBox;
		spinner->setValue(f->getValue());

		layout->addWidget(name, 0, 0);
		layout->addWidget(spinner, 0, 1, Qt::AlignRight);

		this->connect(spinner, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));
	}

	void QIntegerFieldWidget::changeValue(int value)
	{
		VarField<int>* f = TypeInfo::CastPointerDown<VarField<int>>(m_field);
		if (f == nullptr)
		{
			return;
		}

		f->setValue(value);
		f->update();

		emit fieldChanged();
	}


	QRealFieldWidget::QRealFieldWidget(Field* field)
		: QGroupBox()
	{
		m_field = field;
		
		this->setStyleSheet("border:none");
		QGridLayout* layout = new QGridLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		this->setLayout(layout);

		QLabel* name = new QLabel();
		name->setFixedSize(160, 18);
		name->setText(FormatFieldWidgetName(field->getObjectName()));

		QDoubleSlider* slider = new QDoubleSlider;
		//slider->setFixedSize(80,18);
		//slider->setRange(m_field->getMin(), m_field->getMax());
		slider->setRange(0, 1);

		QLabel* spc = new QLabel();
		spc->setFixedSize(10, 18);

		QDoubleSpinner* spinner = new QDoubleSpinner;
		spinner->setFixedSize(100, 18);
		//spinner->setRange(m_field->getMin(), m_field->getMax());
		spinner->setRange(0, 1);

		layout->addWidget(name, 0, 0);
		layout->addWidget(slider, 0, 1);
		layout->addWidget(spc, 0, 2);
		layout->addWidget(spinner, 0, 3, Qt::AlignRight);

		QObject::connect(slider, SIGNAL(valueChanged(double)), spinner, SLOT(setValue(double)));
		QObject::connect(spinner, SIGNAL(valueChanged(double)), slider, SLOT(setValue(double)));
		QObject::connect(spinner, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));

		std::string template_name = field->getTemplateName();
		if (template_name == std::string(typeid(float).name()))
		{
			VarField<float>* f = TypeInfo::CastPointerDown<VarField<float>>(m_field);
			slider->setValue((double)f->getValue());
		}
		else if(template_name == std::string(typeid(double).name()))
		{
			VarField<double>* f = TypeInfo::CastPointerDown<VarField<double>>(m_field);
			slider->setValue(f->getValue());
		}

		FormatFieldWidgetName(field->getObjectName());
	}

	void QRealFieldWidget::changeValue(double value)
	{
		std::string template_name = m_field->getTemplateName();

		if (template_name == std::string(typeid(float).name()))
		{
			VarField<float>* f = TypeInfo::CastPointerDown<VarField<float>>(m_field);
			f->setValue((float)value);
			f->update();
		}
		else if (template_name == std::string(typeid(double).name()))
		{
			VarField<double>* f = TypeInfo::CastPointerDown<VarField<double>>(m_field);
			f->setValue(value);
			f->update();
		}

		emit fieldChanged();
	}


	QVector3FieldWidget::QVector3FieldWidget(Field* field)
		: QGroupBox()
	{
		m_field = field;

		this->setStyleSheet("border:none");
		QGridLayout* layout = new QGridLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		this->setLayout(layout);

		QLabel* name = new QLabel();
		name->setFixedSize(160, 18);
		name->setText(FormatFieldWidgetName(field->getObjectName()));

		spinner1 = new QDoubleSpinner;
		spinner1->setRange(m_field->getMin(), m_field->getMax());

		spinner2 = new QDoubleSpinner;
		spinner2->setRange(m_field->getMin(), m_field->getMax());

		spinner3 = new QDoubleSpinner;
		spinner3->setRange(m_field->getMin(), m_field->getMax());

		layout->addWidget(name, 0, 0);
		layout->addWidget(spinner1, 0, 1);
		layout->addWidget(spinner2, 0, 2);
		layout->addWidget(spinner3, 0, 3);


		std::string template_name = m_field->getTemplateName();

		double v1 = 0;
		double v2 = 0;
		double v3 = 0;

		if (template_name == std::string(typeid(Vector3f).name()))
		{
			VarField<Vector3f>* f = TypeInfo::CastPointerDown<VarField<Vector3f>>(m_field);
			auto v = f->getValue();
			v1 = v[0];
			v2 = v[1];
			v3 = v[2];
		}
		else if (template_name == std::string(typeid(Vector3d).name()))
		{
			VarField<Vector3d>* f = TypeInfo::CastPointerDown<VarField<Vector3d>>(m_field);
			auto v = f->getValue();

			v1 = v[0];
			v2 = v[1];
			v3 = v[2];
		}

		spinner1->setValue(v1);
		spinner2->setValue(v2);
		spinner3->setValue(v3);

		QObject::connect(spinner1, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));
		QObject::connect(spinner2, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));
		QObject::connect(spinner3, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));
	}


	void QVector3FieldWidget::changeValue(double value)
	{
		double v1 = spinner1->value();
		double v2 = spinner2->value();
		double v3 = spinner3->value();

		std::string template_name = m_field->getTemplateName();

		if (template_name == std::string(typeid(Vector3f).name()))
		{
			VarField<Vector3f>* f = TypeInfo::CastPointerDown<VarField<Vector3f>>(m_field);
			f->setValue(Vector3f((float)v1, (float)v2, (float)v3));
			f->update();
		}
		else if (template_name == std::string(typeid(Vector3d).name()))
		{
			VarField<Vector3d>* f = TypeInfo::CastPointerDown<VarField<Vector3d>>(m_field);
			f->setValue(Vector3d(v1, v2, v3));
			f->update();
		}

		emit fieldChanged();
	}

	//QWidget-->QVBoxLayout-->QScrollArea-->QWidget-->QGridLayout
	PPropertyWidget::PPropertyWidget(QWidget *parent)
		: QWidget(parent)
		, m_main_layout()
	{
		m_main_layout = new QVBoxLayout;
		m_scroll_area = new QScrollArea;

		m_main_layout->setContentsMargins(0, 0, 0, 0);
		m_main_layout->setSpacing(0);
		m_main_layout->addWidget(m_scroll_area);

		m_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		m_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		m_scroll_area->setWidgetResizable(true);

		m_scroll_layout = new QGridLayout;
		m_scroll_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

		QWidget * m_scroll_widget = new QWidget;
		m_scroll_widget->setLayout(m_scroll_layout);
		
		m_scroll_area->setWidget(m_scroll_widget);


		setMinimumWidth(250);
		setLayout(m_main_layout);
	}

	PPropertyWidget::~PPropertyWidget()
	{
		m_widgets.clear();
	}

	QSize PPropertyWidget::sizeHint() const
	{
		return QSize(20, 20);
	}

	QWidget* PPropertyWidget::addWidget(QWidget* widget)
	{
		m_scroll_layout->addWidget(widget);
		m_widgets.push_back(widget);

		return widget;
	}

	void PPropertyWidget::removeAllWidgets()
	{
		//TODO: check whether m_widgets[i] should be explicitly deleted
		for (int i = 0; i < m_widgets.size(); i++)
		{
			m_scroll_layout->removeWidget(m_widgets[i]);
			delete m_widgets[i];
		}
		m_widgets.clear();
	}

	void PPropertyWidget::showProperty(Module* module)
	{
//		clear();

		updateContext(module);
	}

	void PPropertyWidget::showProperty(Node* node)
	{
//		clear();

		updateContext(node);
	}

	void PPropertyWidget::showBlockProperty(QtNodes::QtBlock& block)
	{
		auto dataModel = block.nodeDataModel();

		auto node = dynamic_cast<QtNodes::QtNodeWidget*>(dataModel);
		if (node != nullptr)
		{
			this->showProperty(node->getNode().get());
		}
		else
		{
			auto module = dynamic_cast<QtNodes::QtModuleWidget*>(dataModel);
			if (module != nullptr)
			{
				this->showProperty(module->getModule());
			}
		}
	}

	void PPropertyWidget::updateDisplay()
	{
//		PVTKOpenGLWidget::getCurrentRenderer()->GetActors()->RemoveAllItems();
		SceneGraph::getInstance().draw();
		PVTKOpenGLWidget::getCurrentRenderer()->GetRenderWindow()->Render();
	}

	void PPropertyWidget::updateContext(Base* base)
	{
		if (base == nullptr)
		{
			return;
		}

		this->removeAllWidgets();

		std::vector<Field*>& fields = base->getAllFields();

		for each (Field* var in fields)
		{
			if (var != nullptr)
			{
				if (var->getClassName() == std::string("Variable"))
				{
					this->addScalarFieldWidget(var);
				}
			}
		}
	}

	void PPropertyWidget::addScalarFieldWidget(Field* field)
	{
		std::string template_name = field->getTemplateName();
		if (template_name == std::string(typeid(bool).name()))
		{
			auto fw = new QBoolFieldWidget(field);
			this->connect(fw, SIGNAL(fieldChanged()), this, SLOT(updateDisplay()));

			this->addWidget(fw);
		}
		else if (template_name == std::string(typeid(int).name()))
		{
			auto fw = new QIntegerFieldWidget(field);
			this->connect(fw, SIGNAL(fieldChanged()), this, SLOT(updateDisplay()));

			this->addWidget(fw);
//			this->addWidget(new QIntegerFieldWidget(new VarField<int>()));
		}
		else if (template_name == std::string(typeid(float).name()))
		{
			this->addWidget(new QRealFieldWidget(field));
		}
		else if (template_name == std::string(typeid(Vector3f).name()))
		{
			this->addWidget(new QVector3FieldWidget(field));
		}
	}

	void PPropertyWidget::addArrayFieldWidget(Field* field)
	{

	}

}
